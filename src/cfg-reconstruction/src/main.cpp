#include "export.h"
#include "tinyformat.h"

#include <stdexcept>

int main(int argc, char* argv[])
{
  try {
    if (argc < 3) throw std::logic_error("please run as: cfgrecon protobuf_trace_file(s) dot_cfg_file");

    for (auto idx = 1; idx < argc - 1; ++idx) {
      auto pb_trace_file = std::string(argv[idx]);

      auto trace = parse_instructions_from_file(pb_trace_file);
      add_trace_into_basic_block_cfg(trace);
    }

    auto bb_cfg_file = std::string(argv[2]);
    save_basic_block_cfg_to_dot_file(bb_cfg_file);
  }
  catch (const std::exception& expt) {
    tfm::printfln("%s", expt.what());
  }

  return 0;
  (void)argc; (void)argv; // compiler pacifying
}

