#include "instruction.h"

#include <algorithm>

static char disasm_buffer[100];

instruction::instruction(ADDRINT ins_addr, const char* opcode_buffer, int opcode_buffer_size)
{
  this->address = ins_addr;

  auto xed_inst = xed_decoded_inst_t{};
  xed_decoded_inst_zero(&xed_inst);
  xed_decoded_inst_set_mode(&xed_inst, XED_MACHINE_MODE_LEGACY_32, XED_ADDRESS_WIDTH_32b);

  auto decode_err = xed_decode(&xed_inst, XED_STATIC_CAST(const xed_uint8_t*, opcode_buffer), opcode_buffer_size);
  if (decode_err != XED_ERROR_NONE) throw static_cast<uint32_t>(decode_err);

  std::fill_n(disasm_buffer, 100, 0);
//  xed_decoded_inst_dump_xed_format(&xed_inst, disasm_buffer, 100, ins_addr);
  auto disasm_err = xed_format_context(XED_SYNTAX_INTEL, &xed_inst, disasm_buffer, 100, ins_addr, 0);
  if (disasm_err != 0) throw uint32_t{disasm_err};

  this->disassemble = std::string(disasm_buffer);

  this->category = xed_decoded_inst_get_category(&xed_inst);
  this->is_call = (this->category == XED_CATEGORY_CALL);
  this->is_branch = (this->category == XED_CATEGORY_COND_BR);
  this->is_ret = (this->category == XED_CATEGORY_RET);

  this->iclass = xed_decoded_inst_get_iclass(&xed_inst);

  auto xi = xed_decoded_inst_inst(&xed_inst);
  auto ins_noperands = xed_inst_noperands(xi);

  for (decltype(ins_noperands) idx = 0; idx < ins_noperands; ++idx) {
    auto ins_operand = xed_inst_operand(xi, idx);
    auto operand_name = xed_operand_name(ins_operand);

    if (xed_operand_is_register(operand_name)) {
      if (xed_operand_read(ins_operand)) {
        auto xed_read_reg = xed_decoded_inst_get_reg(&xed_inst, operand_name);
        this->src_registers.push_back(INS_XedExactMapToPinReg(xed_read_reg));
      }

      if (xed_operand_written(ins_operand)) {
        auto xed_written_reg = xed_decoded_inst_get_reg(&xed_inst, operand_name);
        this->dst_registers.push_back(INS_XedExactMapToPinReg(xed_written_reg));
      }
    }
  }

  this->is_memory_read = false; this->is_memory_write = false;
  auto ins_mem_noperands = xed_decoded_inst_number_of_memory_operands(&xed_inst);
  for (decltype(ins_mem_noperands) mem_idx = 0; mem_idx < ins_mem_noperands; ++mem_idx) {
    if (xed_decoded_inst_mem_read(&xed_inst, mem_idx)) this->is_memory_read = true;
    if (xed_decoded_inst_mem_written(&xed_inst, mem_idx)) this->is_memory_write = true;
  }

//  this->address     = INS_Address(ins);
//  this->next_address = INS_NextAddress(ins);
////  this->opcode      = INS_Mnemonic(ins);
//  this->opcode_size = INS_Size(ins);
//  this->disassemble = INS_Disassemble(ins);

//  // including image, routine
//  auto img                = IMG_FindByAddress(this->address);
//  this->including_image   = IMG_Valid(img) ? IMG_Name(img) : "";
////  this->including_routine = RTN_FindNameByAddress(this->address);

//  PIN_LockClient();
//  auto routine = RTN_FindByAddress(this->address);
//  PIN_UnlockClient();

//  if (RTN_Valid(routine)) {
//    auto routine_mangled_name = RTN_Name(routine);
//    this->including_routine_name = PIN_UndecorateSymbolName(routine_mangled_name, UNDECORATION_NAME_ONLY);
//  }
//  else this->including_routine_name = "";

//  // has fall through
//  this->has_fall_through = INS_HasFallThrough(ins);

//  // is call, ret or syscall
//  this->is_call    = INS_IsCall(ins);
//  this->is_branch  = INS_IsBranch(ins);
//  this->is_ret     = INS_IsRet(ins);
//  this->is_syscall = INS_IsSyscall(ins);

//  this->category = static_cast<xed_category_enum_t>(INS_Category(ins));
//  this->iclass = static_cast<xed_iclass_enum_t>(INS_Opcode(ins));

//  // read registers
//  auto read_reg_number = INS_MaxNumRRegs(ins);
//  for (decltype(read_reg_number) reg_id = 0; reg_id < read_reg_number; ++reg_id) {
//    this->src_registers.push_back(INS_RegR(ins, reg_id));
//  }

//  // written registers
//  auto written_reg_number = INS_MaxNumWRegs(ins);
//  for (decltype(written_reg_number) reg_id = 0; reg_id < written_reg_number; ++reg_id) {
//    this->dst_registers.push_back(INS_RegW(ins, reg_id));
//  }

//  auto is_special_reg = [](const REG& reg) -> bool {
//    return (reg >= REG_MM_BASE);
//  };

//  this->is_special = std::any_of(std::begin(this->src_registers), std::end(this->src_registers), is_special_reg) ||
//      std::any_of(std::begin(this->dst_registers), std::end(this->dst_registers), is_special_reg) ||
//      (this->category == XED_CATEGORY_X87_ALU) || (this->category == XED_CATEGORY_LOGICAL_FP) ||
//      (this->iclass == XED_ICLASS_XEND) || (this->iclass == XED_ICLASS_PUSHA) ||
//      (this->iclass == XED_ICLASS_PUSHAD) || (this->iclass == XED_ICLASS_PUSHF) ||
//      (this->iclass == XED_ICLASS_PUSHFD) || (this->iclass == XED_ICLASS_PUSHFQ) ||
//      (this->iclass == XED_ICLASS_RDTSC) || (this->iclass == XED_ICLASS_SKINIT) || (this->iclass == XED_ICLASS_RDPMC);

//  // is memory read, write
//  this->is_memory_read    = INS_IsMemoryRead(ins);
//  this->is_memory_write   = INS_IsMemoryWrite(ins);
//  this->has_memory_read_2 = INS_HasMemoryRead2(ins);

}
