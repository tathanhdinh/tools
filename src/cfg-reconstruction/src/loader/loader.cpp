#include "../type/trace.pb.h"
#include "../type/instruction.h"
#include "../type/trace.h"

#include "../tinyformat.h"

#include <memory>
#include <fstream>

dyn_inss_t trace                             = dyn_inss_t();
map_address_instruction_t cached_ins_at_addr = map_address_instruction_t();

std::ifstream protobuf_trace_file;


static auto parse_trace_header () -> void
{
  auto header_size = int{0};
  try {
    protobuf_trace_file.read(reinterpret_cast<char*>(&header_size), sizeof(decltype(header_size)));

    auto header_buffer = std::shared_ptr<char>(new char[header_size], std::default_delete<char[]>());
    protobuf_trace_file.read(header_buffer.get(), header_size);
  }
  catch (const std::exception& expt) {
    tfm::printfln("fatal exception: %s", expt.what());
    std::exit(1);
  }

  return;
}


static auto parse_chunk_from_buffer (const char* buffer, int buffer_size) -> void
{
  auto inst_chunk = trace_format::chunk_t();
  if (!inst_chunk.ParseFromArray(buffer, buffer_size)) throw 1;

  for (auto i = 0; i < inst_chunk.body_size(); ++i) {
    const auto& body = inst_chunk.body(i);
    if (body.typeid_() == trace_format::INSTRUCTION) {
      const auto& pb_inst_info = body.instruction();

      const auto& pb_inst_addr = pb_inst_info.address();
      auto ins_addr = pb_inst_addr.value_32();

      if (cached_ins_at_addr.find(ins_addr) != std::end(cached_ins_at_addr)) {
        const auto& pb_inst_opcode = pb_inst_info.opcode();
        auto opcode_size = pb_inst_opcode.size();
        auto opcode_buffer = pb_inst_opcode.data();

        cached_ins_at_addr[ins_addr] = std::make_shared<instruction>(ins_addr, opcode_buffer, opcode_size);
      }
    }
  }

  return;
}


static auto parse_trace_chunks () -> void
{
  auto chunk_size = int{0};
  try {
    protobuf_trace_file.read(reinterpret_cast<char*>(&chunk_size), sizeof(decltype(chunk_size)));

    auto chunk_buffer = std::shared_ptr<char>(new char[chunk_size], std::default_delete<char[]>());
    protobuf_trace_file.read(chunk_buffer.get(), chunk_size);

    parse_chunk_from_buffer(chunk_buffer.get(), chunk_size);
  }
  catch (uint8_t expt_code) {
    tfm::printfln("parsing message failed, error code %d\n", expt_code);
    std::exit(1);
  }
  catch (const std::exception& expt) {
    tfm::printfln("catched exception: %s", expt.what());
  }

  google::protobuf::ShutdownProtobufLibrary();

  return;
}
