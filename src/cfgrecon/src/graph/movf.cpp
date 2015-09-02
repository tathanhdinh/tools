#include "graph.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include "../tinyformat.h"

extern map_address_instruction_t cached_ins_at_addr;

static bb_graph_t internal_movf_bb_cfg;

static uint32_t target_identifier;
static uint32_t branch_identifier;

static auto is_bb_starting (p_instruction_t ins_a, p_instruction_t ins_b) -> bool
{
  auto result = false;
  if (ins_a->is_memory_read && !ins_a->static_load_addresses.empty()) {
    auto load_addr = ins_a->static_load_addresses.front();
    if (load_addr == target_identifier) {
      if (ins_b->is_immediate_read && ins_b->immediate_read_value == ins_a->address) result = true;
    }
  }

  return result;
}


static auto extract_bb_from_trace (p_instructions_t& trace) -> tr_vertices_t
{
  auto new_bb = tr_vertices_t{};
  new_bb.push_back(trace.at(0)->address); new_bb.push_back(trace.at(1)->address);

  auto last_bb_ins_idx = uint32_t{2};
  try {
    for (;;) {
      auto ins_a = trace.at(last_bb_ins_idx); last_bb_ins_idx++;
      auto ins_b = trace.at(last_bb_ins_idx); last_bb_ins_idx++;
      if (is_bb_starting(ins_a, ins_b)) {
        last_bb_ins_idx -= 2;
        throw std::logic_error("new basic block reached");
      }
    }
  }
  catch (const std::exception& expt) {
    for (auto idx = decltype(last_bb_ins_idx){2}; idx < last_bb_ins_idx; ++idx) {
      new_bb.push_back(trace.at(idx)->address);
    }
    trace.erase(std::begin(trace), std::begin(trace) + last_bb_ins_idx);
  }

  return new_bb;
}

static auto extract_basic_blocks_from_program (const p_instructions_t& prog) -> void
{
  auto local_trace = prog;
  auto bb_idx = uint32_t{0};

  while (local_trace.size() > 2) {
    auto new_bb = extract_bb_from_trace(local_trace);
    boost::add_vertex(bb_vertex_t(bb_idx, new_bb), internal_movf_bb_cfg);
  }

  return;
}


static auto is_branching (uint32_t addr_a, uint32_t addr_b) -> bool
{
  auto ins_a = cached_ins_at_addr[addr_a];
  auto ins_b = cached_ins_at_addr[addr_b];

  auto result = false;
  if (ins_a->is_immediate_read && ins_a->immediate_read_value == branch_identifier) {
    if (ins_b->is_memory_write && !ins_b->static_store_addresses.empty()) {
      auto store_addr = ins_b->static_store_addresses.front();
      if (store_addr == branch_identifier) result = true;
    }
  }

  return result;
}


static auto branch_target (uint32_t addr_a, uint32_t addr) -> uint32_t
{
  auto target = cached_ins_at_addr[addr_a]->immediate_read_value;
  if ((target & 0x10000000) != 0x0) target &= 0x00ffffff;
  return target;
}


static auto parse_target_branches_from_bb (const tr_vertices_t& bb) -> std::vector<uint32_t>
{
  auto target_addrs = std::vector<uint32_t>{};

  try {
    auto target_addr = uint32_t{0};
    auto ins_idx = 0;

    for (;;) {
      auto addr_a = bb.at(ins_idx); ++ins_idx;
      auto addr_b = bb.at(ins_idx); ++ins_idx;
      if (is_branching(addr_a, addr_b)) {
        target_addr = branch_target(addr_a, addr_b);
        target_addrs.push_back(target_addr);
      }
    }
  }
  catch (const std::exception& expt) {
  }

  return target_addrs;
}


static auto get_vertex_from_target_address (uint32_t addr) -> bb_vertex_desc_t
{
  auto first_vertex_iter = bb_vertex_iter_t{};
  auto last_vertex_iter = bb_vertex_iter_t{};

  auto result_vertex_desc = bb_graph_t::null_vertex();

  std::tie(first_vertex_iter, last_vertex_iter) = boost::vertices(internal_movf_bb_cfg);
  for (auto vertex_iter = first_vertex_iter; vertex_iter != last_vertex_iter; ++vertex_iter) {
    if (internal_movf_bb_cfg[*vertex_iter].second.front() == addr) {
      result_vertex_desc = *vertex_iter;
      break;
    }
  }

  return result_vertex_desc;
}


static auto extract_branching_between_basic_blocks () -> void
{
  auto first_vertex_iter = bb_vertex_iter_t{};
  auto last_vertex_iter = bb_vertex_iter_t{};

  std::tie(first_vertex_iter, last_vertex_iter) = boost::vertices(internal_movf_bb_cfg);
  for (auto vertex_iter = first_vertex_iter; vertex_iter != last_vertex_iter; ++vertex_iter) {
    auto vertex_desc = *vertex_iter;

//    auto vertex_idx = internal_movf_bb_cfg[vertex_desc].first;
    auto vertex_bb = internal_movf_bb_cfg[vertex_desc].second;

    auto target_addrs = parse_target_branches_from_bb(vertex_bb);
    for (auto addr : target_addrs) {
      auto target_vertex = get_vertex_from_target_address(addr);
      assert(target_vertex != bb_graph_t::null_vertex());

      boost::add_edge(vertex_desc, target_vertex, internal_movf_bb_cfg);
    }
  }

  return;
}


static auto write_graph_vertex (std::ostream& label, bb_vertex_desc_t vertex_desc) -> void
{
  if (std::get<BB_ORDER>(internal_movf_bb_cfg[vertex_desc]) == 0) {
      tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=cornflowerblue,label=\"");
  }
  else if (boost::out_degree(vertex_desc, internal_movf_bb_cfg) == 0) {
    tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=gainsboro,label=\"");
  }
  else if (boost::in_degree(vertex_desc, internal_movf_bb_cfg) > 2) {
    tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=darkorchid1,label=\"");
  }
  else if (boost::out_degree(vertex_desc, internal_movf_bb_cfg) > 2) {
    tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=darkgoldenrod1,label=\"");
  }
  else tfm::format(label, "[shape=box,style=rounded,label=\"");

  tfm::format(label, "%d\n", std::get<BB_ORDER>(internal_movf_bb_cfg[vertex_desc]));
  for (const auto& addr : std::get<BB_ADDRESSES>(internal_movf_bb_cfg[vertex_desc])) {
    /*if (std::addressof(addr) == std::addressof(internal_bb_graph[vertex_desc].back())) {
      tfm::format(label, "%-12s %-s", StringFromAddrint(addr), cached_ins_at_addr[addr]->disassemble);
    }
    else*/
    tfm::format(label, "0x%-12x %-s\\l", addr, cached_ins_at_addr[addr]->disassemble);
  }
  tfm::format(label, "\",fontname=\"Inconsolata\",fontsize=10.0]");

  return;
}


static auto write_graph_thumbnail_vertex (std::ostream& label, bb_vertex_desc_t vertex_desc) -> void
{
  if (std::get<BB_ORDER>(internal_movf_bb_cfg[vertex_desc]) == 0) {
      tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=cornflowerblue,label=\"");
  }
  else if (boost::out_degree(vertex_desc, internal_movf_bb_cfg) == 0) {
    tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=gainsboro,label=\"");
  }
  else if (boost::in_degree(vertex_desc, internal_movf_bb_cfg) > 2) {
    tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=darkorchid1,label=\"");
  }
  else if (boost::out_degree(vertex_desc, internal_movf_bb_cfg) > 2) {
    tfm::format(label, "[shape=box,style=\"filled,rounded\",fillcolor=darkgoldenrod1,label=\"");
  }
  else tfm::format(label, "[shape=box,style=rounded,label=\"");

  tfm::format(label, "%d", std::get<BB_ORDER>(internal_movf_bb_cfg[vertex_desc]));
  tfm::format(label, "\",fontname=\"Inconsolata\",fontsize=10.0]");
  return;
}


static auto write_cfg_edge (std::ostream& label, bb_edge_desc_t edge_desc) -> void
{
  tfm::format(label, "[label=\"\"]");
  return;
}


/* ===================================== exported functions ===================================== */

auto construct_movf_basic_block_cfg (const p_instructions_t& prog) -> void
{
  extract_basic_blocks_from_program(prog);
  extract_branching_between_basic_blocks();
  return;
}

auto save_movf_basic_block_cfg_to_file (const std::string& filename) -> void
{
  std::ofstream output_file(filename.c_str(), std::ofstream::out | std::ofstream::trunc);

  boost::write_graphviz(output_file, internal_movf_bb_cfg,
                        std::bind(write_graph_thumbnail_vertex, std::placeholders::_1, std::placeholders::_2),
                        std::bind(write_cfg_edge, std::placeholders::_1, std::placeholders::_2));

  output_file.close();
  return;
}


