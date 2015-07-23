#include <iostream>

#include "export.h"

int main(int argc, char* argv[])
{
  auto pb_trace_file = std::string(argv[1]);
  print_instructions_parsed_from_file(pb_trace_file);

  return 0;
  (void)argc; (void)argv; // compiler pacifying
}

