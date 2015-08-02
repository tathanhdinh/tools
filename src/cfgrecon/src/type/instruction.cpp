#include "instruction.h"
#include "../tinyformat.h"

#include <string>
#include <algorithm>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

static char disasm_buffer[1000];

static auto parse_disassembled_instruction_from_buffer (const std::string& disasm_buffer) -> std::string
{
  std::vector<std::string> disasm_strs;
  boost::split(disasm_strs, disasm_buffer, boost::is_any_of("\n\r"));
  auto last_str = disasm_strs.back();
  return last_str.substr(6);
}


instruction::instruction(const instruction& other_inst)
{
  this->address = other_inst.address;
  this->next_address = other_inst.next_address;
  this->disassemble = other_inst.disassemble;

  this->is_call = other_inst.is_call;
  this->is_branch = other_inst.is_branch;
  this->is_syscall = other_inst.is_syscall;
  this->is_ret = other_inst.is_ret;
  this->is_uncond_branch = other_inst.is_uncond_branch;

  this->category = other_inst.category;
  this->iclass = other_inst.iclass;

//  this->read_registers = other_inst.read_registers;
//  this->written_registers = other_inst.written_registers;
  this->read_register = other_inst.read_register;
  this->written_register = other_inst.written_register;

//  this->src_mem = other_inst.src_mem;
//  this->dst_mem = other_inst.dst_mem;
  this->load_memory = other_inst.load_memory;
  this->store_memmory = other_inst.store_memmory;

  this->is_memory_read = other_inst.is_memory_read;
  this->is_memory_write = other_inst.is_memory_write;
}


instruction::instruction(uint32_t ins_addr, const char* opcode_buffer, int opcode_buffer_size)
{
  this->address = ins_addr;

  auto xed_inst = xed_decoded_inst_t{};
  xed_decoded_inst_set_mode(&xed_inst, XED_MACHINE_MODE_LEGACY_32, XED_ADDRESS_WIDTH_32b);

  auto decode_err = xed_decode(&xed_inst, XED_STATIC_CAST(const xed_uint8_t*, opcode_buffer), opcode_buffer_size);

  if (decode_err != XED_ERROR_NONE) throw std::logic_error("instruction decode error");

  std::fill_n(disasm_buffer, 1000, 0);
//  xed_decoded_inst_dump_xed_format(&xed_inst, disasm_buffer, 118, ins_addr);
//  auto disasm_err = xed_format_context(XED_SYNTAX_INTEL, &xed_inst, disasm_buffer, 128, ins_addr, nullptr);
//  this->disassemble = std::string(disasm_buffer);

  xed_decoded_inst_dump(&xed_inst, disasm_buffer, 1000);
  this->disassemble = parse_disassembled_instruction_from_buffer(disasm_buffer);

  this->category = xed_decoded_inst_get_category(&xed_inst);
  this->is_call = (this->category == XED_CATEGORY_CALL);
  this->is_branch = (this->category == XED_CATEGORY_COND_BR);
  this->is_ret = (this->category == XED_CATEGORY_RET);
  this->is_uncond_branch = (this->category == XED_CATEGORY_UNCOND_BR);

  this->iclass = xed_decoded_inst_get_iclass(&xed_inst);

  auto xi = xed_decoded_inst_inst(&xed_inst);
//  auto ins_operand_num = xed_inst_noperands(xi);
  auto ins_operand_num = xed_decoded_inst_noperands(&xed_inst);

//  for (auto idx = decltype(ins_operand_num){0}; idx < ins_operand_num; ++idx) {
//    auto ins_operand = xed_inst_operand(xi, idx);
//    auto operand_name = xed_operand_name(ins_operand);

//    if (xed_operand_is_register(operand_name)) {
//      if (xed_operand_read(ins_operand)) {
//        auto xed_read_reg = xed_decoded_inst_get_reg(&xed_inst, operand_name);
////        this->src_registers.push_back(xed_read_reg);
//        this->read_registers[xed_read_reg] = 0x0;
//      }

//      if (xed_operand_written(ins_operand)) {
//        auto xed_written_reg = xed_decoded_inst_get_reg(&xed_inst, operand_name);
////        this->dst_registers.push_back(xed_written_reg);
////        INS_XedExactMapToPinReg(xed_written_reg);
//        this->written_registers[xed_written_reg] = 0x0;
//      }
//    }
//  }

//  tfm::printfln("%s src: %d dst: %d", this->disassemble, this->read_registers.size(), this->written_registers.size());

  this->is_memory_read = false; this->is_memory_write = false;
  auto ins_mem_noperands = xed_decoded_inst_number_of_memory_operands(&xed_inst);
  for (decltype(ins_mem_noperands) mem_idx = 0; mem_idx < ins_mem_noperands; ++mem_idx) {
    if (xed_decoded_inst_mem_read(&xed_inst, mem_idx)) this->is_memory_read = true;
    if (xed_decoded_inst_mem_written(&xed_inst, mem_idx)) this->is_memory_write = true;
  }
}
