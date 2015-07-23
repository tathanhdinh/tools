#ifndef EXPORT_H
#define EXPORT_H

#include <string>

auto normalize_hex_string (const std::string& input) -> std::string;
auto print_instructions_parsed_from_file (const std::string& filename) -> void;

#endif // EXPORT_H

