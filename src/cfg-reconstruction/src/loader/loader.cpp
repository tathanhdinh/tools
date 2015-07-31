#include "../type/trace.pb.h"
#include "../type/instruction.h"

#include "../tinyformat.h"

#include <memory>
#include <fstream>

static p_instructions_t trace = p_instructions_t{};
map_address_instruction_t cached_ins_at_addr = map_address_instruction_t();

static std::ifstream protobuf_trace_file;

static auto parse_trace_header () -> void
{
  auto header_size = int{0};
  protobuf_trace_file.read(reinterpret_cast<char*>(&header_size), sizeof(decltype(header_size)));
  if (!protobuf_trace_file) throw std::range_error("reading header size error");

  auto header_buffer = std::shared_ptr<char>(new char[header_size], std::default_delete<char[]>());
  protobuf_trace_file.read(header_buffer.get(), header_size);
  if (!protobuf_trace_file) throw std::range_error("reading header error");

  return;
}

static auto inst_chunk = trace_format::chunk_t();
static auto parse_chunk_from_buffer (const char* buffer, int buffer_size) -> void
{
//  auto inst_chunk = trace_format::chunk_t();
  if (!inst_chunk.ParseFromArray(buffer, buffer_size)) throw std::domain_error("parsing chunk error");

//  tfm::printfln("number of instructions in chunk: %d", inst_chunk.body_size());
  auto body_num = inst_chunk.body_size();
  for (auto i = 0; i < body_num; ++i) {
    const auto& body = inst_chunk.body(i);

    if (body.typeid_() == trace_format::INSTRUCTION) {
      const auto& pb_inst_info = body.instruction();

      const auto& pb_inst_addr = pb_inst_info.address();
      auto ins_addr = pb_inst_addr.value_32();

      if (cached_ins_at_addr.find(ins_addr) == std::end(cached_ins_at_addr)) {
        const auto& pb_inst_opcode = pb_inst_info.opcode();
        auto opcode_size = pb_inst_opcode.size();
        auto opcode_buffer = pb_inst_opcode.data();

        cached_ins_at_addr[ins_addr] = std::make_shared<instruction>(ins_addr, opcode_buffer, opcode_size);
      }

      trace.push_back(cached_ins_at_addr[ins_addr]);

//      const auto& con_info_num = pb_inst_info.concrete_info_size();
//      for (auto idx = 0; idx < con_info_num; ++idx) {
//        const auto& con_info_idx = pb_inst_info.concrete_info(idx);
//        if (con_info_idx.typeid_() == trace_format::NEXT_ADDRESS) {
//          auto pb_next_addr = con_info_idx.next_address();
//          auto next_addr = pb_next_addr.value_32();
//          tfm::printfln("next address 0x%x", next_addr);
//        }
//      }
    }
  }

  inst_chunk.Clear();

  return;
}


static auto parse_trace_chunks () -> void
{
  auto chunk_size = int{0};

  while (true) {
    protobuf_trace_file.read(reinterpret_cast<char*>(&chunk_size), sizeof(decltype(chunk_size)));
    if (!protobuf_trace_file) throw std::range_error("reading chunk size error");

    auto chunk_buffer = std::shared_ptr<char>(new char[chunk_size], std::default_delete<char[]>());
    protobuf_trace_file.read(chunk_buffer.get(), chunk_size);

//    tfm::printfln("chunk size: %d bytes", chunk_size);
    parse_chunk_from_buffer(chunk_buffer.get(), chunk_size);
  }

  return;
}


/* ===================================== exported functions ===================================== */

auto print_instructions_parsed_from_file (const std::string& filename) -> void
{
  try {
//    protobuf_trace_file = std::ifstream(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    protobuf_trace_file.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);

    xed_tables_init();

    parse_trace_header();
    parse_trace_chunks();

    for (const auto& addr_inst : cached_ins_at_addr) {
      tfm::printfln("0x%x %s", std::get<0>(addr_inst), std::get<1>(addr_inst)->disassemble);
    }
  }
  catch (const std::exception& expt) {
    tfm::printfln("%s", expt.what());
  }

  google::protobuf::ShutdownProtobufLibrary();
  protobuf_trace_file.close();

  return;
}

auto parse_instructions_from_file (const std::string& filename) -> const p_instructions_t&
{
  trace.clear();

  try {
    tfm::printfln("===== reading protobuf trace (input file: %s)...", filename);
//    protobuf_trace_file = std::move(std::ifstream(filename.c_str(), std::ifstream::in | std::ifstream::binary));
    protobuf_trace_file.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);

    xed_tables_init();

    parse_trace_header();
    parse_trace_chunks();
  }
  catch (const std::exception& expt) {
    tfm::printfln("%s instruction parsed", trace.size());
  }

  google::protobuf::ShutdownProtobufLibrary();
  protobuf_trace_file.close();

  return trace;
}



