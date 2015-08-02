#include "../type/instruction.h"
#include "../tinyformat.h"

#include <fstream>

#define MEM_LOAD true
#define MEM_STORE false
template<bool load_or_store>
static auto get_memory_access_addresses (p_instructions_t trace) -> std::vector<uint32_t>
{
  auto access_mem_addrs = std::vector<uint32_t>{};

  for (const auto& ins : trace) {
    const auto& access_mem = load_or_store ? ins->load_memory : ins->store_memmory;

    for (const auto& mem_addr_val : access_mem) {
      access_mem_addrs.push_back(std::get<0>(mem_addr_val));
    }
  }

  return access_mem_addrs;
}


static auto split_trace_into_chunks_of_mov_instructions (p_instructions_t trace) -> std::vector<p_instructions_t>
{
  auto ins_chunks = std::vector<p_instructions_t>{};

  auto begin_iter = std::begin(trace);
  auto last_iter = std::end(trace); --last_iter;

  for (auto ins_iter = std::begin(trace); ins_iter != std::end(trace); ++ins_iter) {
    if ((*ins_iter)->is_uncond_branch) {
      ins_chunks.push_back(p_instructions_t(begin_iter, ins_iter));
      begin_iter = ins_iter; ++begin_iter;
    }
    else if (ins_iter == last_iter) {
      ins_chunks.push_back(p_instructions_t(begin_iter, std::end(trace)));
    }
  }

  return ins_chunks;
}


/* ===================================== exported functions ===================================== */

auto save_memory_access_of_trace_to_file (const p_instructions_t& trace, const std::string& filename) -> void
{
  try {
    std::ofstream output_file(filename.c_str(), std::ofstream::trunc);
    if (!output_file) throw std::logic_error("cannot open output file");

    auto ins_chunks = split_trace_into_chunks_of_mov_instructions(trace);
    for (auto chunk : ins_chunks) {
      auto load_addresses = get_memory_access_addresses<MEM_LOAD>(chunk);
      auto store_addresses = get_memory_access_addresses<MEM_STORE>(chunk);

      tfm::format(output_file, "=====\n");
      tfm::format(output_file, "load addresses: ");
      for (auto load_addr : load_addresses) {
        tfm::printf("0x%x ", load_addr);
      }

      tfm::format(output_file, "\nstore addresses: ");
      for (auto store_addr : store_addresses) {
        tfm::printf("0x%x ", store_addr);
      }

      tfm::format(output_file, "\n");
    }

  }
  catch (const std::exception& expt) {
    tfm::printfln("%s", expt.what());
  }
}
