#ifndef EXPORT_H
#define EXPORT_H

#include <string>

auto parse_instructions_from_file         (const std::string& filename) -> void;
auto cap_save_basic_block_cfg_to_dot_file (const std::string& filename) -> void;

#endif // EXPORT_H

