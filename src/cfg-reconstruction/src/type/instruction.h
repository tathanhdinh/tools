
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../parsing_helper.h"
//#include <pin.H>
extern "C" {
#include "xed-interface.h"
}

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <map>

class instruction
{
//public:
//  static const xed_machine_mode_enum_t machine_mode = XED_MACHINE_MODE_LEGACY_32;
//  static const xed_address_width_enum_t address_with = XED_ADDRESS_WIDTH_32b;

 public:
  uint32_t    address;
  uint32_t    next_address;
  std::string disassemble;

  bool has_fall_through;

  bool is_call;
  bool is_branch;
  bool is_syscall;
  bool is_ret;
//  bool is_special;

  xed_category_enum_t category;
  xed_iclass_enum_t iclass;

  std::vector<xed_reg_enum_t> src_registers;
  std::vector<xed_reg_enum_t> dst_registers;

  bool is_memory_read;
  bool is_memory_write;

 public:
  instruction(uint32_t ins_addr, const char* opcode_buffer, int opcode_buffer_size);
};

//typedef std::shared_ptr<instruction> p_instruction_t;
//typedef std::vector<p_instruction_t> p_instructions_t;
//typedef std::map<ADDRINT, p_instruction_t> map_address_instruction_t;
//typedef std::shared_ptr<map_address_instruction_t> p_map_address_instruction_t;
using p_instruction_t             = std::shared_ptr<instruction>;
using p_instructions_t            = std::vector<p_instruction_t>;
using map_address_instruction_t   = std::map<uint32_t, p_instruction_t>;
using p_map_address_instruction_t = std::shared_ptr<map_address_instruction_t>;

#endif
















