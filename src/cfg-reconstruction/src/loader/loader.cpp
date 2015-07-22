#include "../type/trace.pb.h"
#include "../type/instruction.h"
#include "../type/trace.h"

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
    tfm::printfln("catched exception: %s", expt.what());
    std::exit(1);
  }

  return;
}


static auto parse_trace_chunks () -> void
{
  auto trunk_size = int{0};
  try {
    protobuf_trace_file.read(reinterpret_cast<char*>(&trunk_size), sizeof(decltype(trunk_size)));
  }
  catch (const std::exception& expt) {
    tfm::printfln("catched exception: %s", expt.what());
  }
}
