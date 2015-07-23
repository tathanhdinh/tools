#include "export.h"
#include "tinyformat.h"

#include <stdexcept>

int main(int argc, char* argv[])
{
  try {
    if (argc != 3) throw std::logic_error("please run as: cfgrecon protobuf_trace_file dot_cfg_file");

    auto pb_trace_file = std::string(argv[1]);
    parse_instructions_from_file(pb_trace_file);

    auto bb_cfg_file = std::string(argv[2]);
    cap_save_basic_block_cfg_to_dot_file(bb_cfg_file);
  }
  catch (const std::exception& expt) {
    tfm::printfln("%s", expt.what());
  }

  return 0;
  (void)argc; (void)argv; // compiler pacifying
}

