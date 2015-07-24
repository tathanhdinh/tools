#ifndef EXPORT_H
#define EXPORT_H

#include "type/instruction.h"

#include <string>

auto parse_instructions_from_file         (const std::string& filename)   -> const p_instructions_t&;
auto construct_basic_block_cfg            ()                              -> void;
auto add_trace_into_basic_block_cfg       (const p_instructions_t& trace) -> void;
auto save_basic_block_cfg_to_dot_file     (const std::string& filename)   -> void;

#endif // EXPORT_H

