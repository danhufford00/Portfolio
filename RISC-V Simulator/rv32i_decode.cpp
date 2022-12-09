#include "rv32i_decode.h"

/**
* This function constructs a memory object by allocating siz bytes into the mem
* vector, the value of each being 0xa5
*
* @param uint32_t addr is the memory address where the insn is stored
*	 uint32_t insn is the value of the insn
*
* @return a string representing the disassembled instruction text
*********************************************************************************/

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn){
	switch(get_opcode(insn)){
        	case opcode_lui:
			return render_lui(insn);
        	case opcode_auipc:
			return render_auipc(insn);
        	case opcode_jal:
			return render_jal(addr, insn);
        	case opcode_jalr:
			return render_jalr(insn);
        	case opcode_btype:
            		switch (get_funct3(insn)){ //funct3 is used to further decode instructions that share an opcode
                		case funct3_beq:
					return render_btype(addr, insn, "beq");
                		case funct3_bne:
					return render_btype(addr, insn, "bne");
                		case funct3_blt:
					return render_btype(addr, insn, "blt");
                		case funct3_bge:
					return render_btype(addr, insn, "bge");
                		case funct3_bltu:
					return render_btype(addr, insn, "bltu");
                		case funct3_bgeu:
					return render_btype(addr, insn, "bgeu");
				default:
					return render_illegal_insn(insn);
            		}
            		assert(0 && "unrecognized opcode"); //It should be impossible to ever get here!
        	case opcode_load_imm:
            		switch (get_funct3(insn)){
                		case funct3_lh:
					return render_itype_load(insn, "lh");
                		case funct3_lb:
					return render_itype_load(insn, "lb");
                		case funct3_lw:
					return render_itype_load(insn, "lw");
                		case funct3_lbu:
					return render_itype_load(insn, "lbu");
                		case funct3_lhu:
					return render_itype_load(insn, "lhu");
				default:
					return render_illegal_insn(insn);
            		}
        	case opcode_stype:
            		switch (get_funct3(insn)){
                		case funct3_sb:
					return render_stype(insn, "sb");
                		case funct3_sh:
					return render_stype(insn, "sh");
                		case funct3_sw:
					return render_stype(insn, "sw");
				default:
					return render_illegal_insn(insn);
            		}
        	case opcode_alu_imm:
            		switch (get_funct3(insn)){
               			case funct3_add:
					return render_itype_alu(insn, "addi", get_imm_i(insn));
          		        case funct3_sll:
					return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
                		case funct3_slt:
					return render_itype_alu(insn, "slti", get_imm_i(insn));
                		case funct3_sltu:
					return render_itype_alu(insn, "sltiu", get_imm_i(insn));
                		case funct3_xor:
					return render_itype_alu(insn, "xori", get_imm_i(insn));
                		case funct3_or:
					return render_itype_alu(insn, "ori", get_imm_i(insn));
                		case funct3_and:
					return render_itype_alu(insn, "andi", get_imm_i(insn));
                		case funct3_srx:
                    			switch(get_funct7(insn)){ //funct7 is used to decode instructions that have the same opcode
                        			case funct7_sra:
							return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
                        			case funct7_srl:
							return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
						default:
							return render_illegal_insn(insn);
                    			}
                    			assert(0 && "unrecognized funct7"); //impossible
				default:
					return render_illegal_insn(insn);
            			}
        	case opcode_rtype:
            		switch(get_funct3(insn)){
                		case funct3_add:
                    			switch(get_funct7(insn)){
                        			case funct7_add:
							return render_rtype(insn, "add");
                        			case funct7_sub:
							return render_rtype(insn, "sub");
                    			}
                		case funct3_srx:
                    			switch(get_funct7(insn)){
                        			case funct7_srl:
							return render_rtype(insn, "srl");
                        			case funct7_sra:
							return render_rtype(insn, "sra");
                    			}
                		case funct3_sll:
					return render_rtype(insn, "sll");
                		case funct3_slt:
					return render_rtype(insn, "slt");
		                case funct3_sltu:
					return render_rtype(insn, "sltu");
                		case funct3_xor:
					return render_rtype(insn, "xor");
                		case funct3_or:
					return render_rtype(insn, "or");
                		case funct3_and:
					return render_rtype(insn, "and");
				default:
					return render_illegal_insn(insn);
            		}
            		assert(0 && "unrecognized opcode"); //It should be impossible to ever get here!
        	case opcode_system:
            		switch(get_funct3(insn)){
                		case funct3_csrrw:
					return render_csrrx(insn, "csrrw");
                		case funct3_csrrs:
					return render_csrrx(insn, "csrrs");
                		case funct3_csrrc:
					return render_csrrx(insn, "csrrc");
                		case funct3_csrrwi:
					return render_csrrxi(insn, "csrrwi");
                		case funct3_csrrsi:
					return render_csrrxi(insn, "csrrsi");
                		case funct3_csrrci:
					return render_csrrxi(insn, "csrrci");
				default:
                                        switch(insn){
                                                case insn_ecall:
                                                        return render_ecall(insn);
                                                case insn_ebreak:
                                                        return render_ebreak(insn);
                                                default:
                                                        return render_illegal_insn(insn);
                                        }
        		}
		default:
                        return render_illegal_insn(insn);
    	}
}

/**
* This functions returns the opcode of a given instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a uint32_t value representing the opcode of the
* given instruction
**********************************************************/

uint32_t rv32i_decode::get_opcode(uint32_t insn){
	return (insn & 0x0000007f); //7 is chosen for the last 7 bits
}

/**
* This function returns the destination register of a
* given instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a uint32_t value representing the destination
* register of the given instruction
**********************************************************/

uint32_t rv32i_decode::get_rd(uint32_t insn){
	return ((insn & 0x00000f80) >> (7-0)); //get bits 7-11 before shifting right 7 bits
}

/**
* This function returns the funct3 field of a given
* instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a uint32_t value representing the funct3 field
* of the given instruction
**********************************************************/

uint32_t rv32i_decode::get_funct3(uint32_t insn){
    	return ((insn & 0x00007000) >> (12-0)); //get bits 12-14 before shifting right 12 bits
}

/**
* This function returns the rs1 field of a given
* instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a uint32_t value representinng the rs1 field of
* the given instruction
**********************************************************/

uint32_t rv32i_decode::get_rs1(uint32_t insn){
	return ((insn & 0x000f8000) >> (15-0)); //get bits 15-18 before shifting right 15 bits
}

/**
* This function returns the rs2 field of a given
* instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a uint32_t value representinng the rs2 field of
* the given instruction
**********************************************************/

uint32_t rv32i_decode::get_rs2(uint32_t insn){
	return ((insn & 0x01f00000) >> (20)); //get bits 20-24 before shifting right 20 bits
}

/**
* This function returns the funct7 field of a given
* instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a uint32_t value representinng the funct7 field
* of the given instruction
**********************************************************/

uint32_t rv32i_decode::get_funct7(uint32_t insn){
	return ((insn & 0xfe000000) >> (25-0)); //get bits 31-25 before shifting right 25 bits
}

/**
* This function returns the imm of an i instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a int32_t value representing the imm of an i
* instruction
**********************************************************/

int32_t rv32i_decode::get_imm_i(uint32_t insn){
	int32_t i = ((insn & 0xfff00000) >> 20); //get bits 31-20 before shifting right 20 bits

    	if (insn & 0x80000000)
        	i |= 0xfffff000; //left sign extension

    	return i;
}

/**
* This function returns the imm of a u instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a int32_t value representing the imm of a u
* instruction
**********************************************************/

int32_t rv32i_decode::get_imm_u(uint32_t insn){
	return (insn & 0xFFFFF000); //get bits 31-12
}

/**
* This function returns the imm of a b instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a int32_t value representing the imm of an b
* instruction
**********************************************************/

int32_t rv32i_decode::get_imm_b(uint32_t insn){
	int32_t b = (insn & 0x7e000000) >> (25-5); //b represents the imm of the given b instruction

	b |= (insn & 0x00000f00) >> (8-1);
	b |= (insn & 0x00000080) << (11-7);
	b &= (0xfffffffe);
	b |= (insn & 0x80000000) >> (31-12);

	if (insn & 0x80000000)
        	b |= 0xfffff000; //left sign extension

    	return b;
}

/**
* This function returns the imm of an s instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a int32_t value representing the imm of an s
* instruction
**********************************************************/

int32_t rv32i_decode::get_imm_s(uint32_t insn){
	int32_t s = (insn & 0xfe000000) >> (25-5); //s represents the imm of the given s instruction

    	s |= (insn & 0x00000f80) >> (7-0);

    	if (insn & 0x80000000)
        	s |= 0xfffff000; //left sign-extension

    	return s;
}

/**
* This function returns the imm of a j instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a int32_t value representing the imm of a j
* instruction
**********************************************************/

int32_t rv32i_decode::get_imm_j(uint32_t insn){
	int32_t j = (insn & 0x7ff00000) >> (30-10); //j represents the imm of the given j instruction

    	j |= ((insn & 0x000ff000) >> (0-0));
    	j |= ((insn & 0x00100000) >> (20-11));
    	j &= (0xfffffffe);
    	j |= ((insn & 0x80000000) >> (31-20));

    	if (insn & 0x80000000)
        	j |= 0xfff00000; //left sign-extension

    	return j;
}

/**
* This function prints a message stating that a given
* address is invalid
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a string holding the appropriate error message
**********************************************************/

std::string rv32i_decode::render_illegal_insn(uint32_t insn){
	(void)insn;
	return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
* This functions formats a given lui instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a string holding a formatted lui instruction
**********************************************************/

std::string rv32i_decode::render_lui(uint32_t insn){
	uint32_t rd = get_rd(insn);
    	int32_t u = get_imm_u(insn);
    	std::ostringstream os;

    	os << render_mnemonic("lui") << render_reg(rd) << "," << hex::to_hex0x20((u >> 12)&0x0fffff);
    	return os.str();
}

/**
* This function formats a given auipc instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a string holding a formatted auipc instruction
**********************************************************/

std::string rv32i_decode::render_auipc(uint32_t insn){
	uint32_t rd = get_rd(insn);
    	int32_t u = get_imm_u(insn);
    	std::ostringstream os;

    	os << render_mnemonic("auipc") << render_reg(rd) << "," << hex::to_hex0x20((u >> 12)&0x0fffff);
    	return os.str();
}

/**
* This function formats a given jal instruction
*
* @param uint32_t addr is the address in memory where the
*  instruction is stored
*        uint32_t insn is a 32 bit instruction
*
* @return a string holding a formatted jal instruction
**********************************************************/

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn){
	uint32_t rd = get_rd(insn);
    	int32_t j = get_imm_j(insn);
    	std::ostringstream os;

    	os << render_mnemonic("jal") << render_reg(rd) << "," << hex::to_hex0x32(j + addr);
    	return os.str();
}

/**
* This function formats a jalr instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a string holding a formatted jalr instruction
**********************************************************/

std::string rv32i_decode::render_jalr(uint32_t insn){
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	int32_t i = get_imm_i(insn);
	std::ostringstream os;

    	os << render_mnemonic("jalr") << render_reg(rd) << "," << i <<  "(" << render_reg(rs1) << ")";
    	return os.str();
}

/**
* This function formats a b type instruction
*
* @param uint32_t addr is the address in memory where the
* instruction is stored
*        uint32_t insn is a 32 bit instruction
* const char *mnemonic is a character array representing
* the instruction's mnemonic
*
* @return a string holding a formatted b type instruction
**********************************************************/

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic){
	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t b = get_imm_b(insn);
    	std::ostringstream os;

   	os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << hex::to_hex0x32(b + addr);
    	return os.str();
}

/**
* This function formats an I type load instruction
*
* @param uint32_t insn is a 32 bit instruction
* const char *mnemonic is a character array representing
* the instruction's mnemonic
*
* @return a string holding a formatted I type load
* instruction
**********************************************************/

std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic){
	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t i = get_imm_i(insn);
    	std::ostringstream os;

    	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << i << "(" << render_reg(rs1) << ")";
    	return os.str();
}

/**
* This function formats an s type instruction
*
* @param uint32_t insn is a 32 bit instruction
* const char *mnemonic is a character array representing
* the instruction's mnemonic
*
* @return a string holding a formatted s type instruction
**********************************************************/

std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic){
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
    	int32_t s = get_imm_s(insn);
    	std::ostringstream os;

    	os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << s << "(" << render_reg(rs1) << ")";
    	return os.str();
}

/**
* This function formats an I type alu instruction
*
* @param uint32_t insn is a 32 bit instruction
* constant char *mnemonic is a character array representing
* the instruction's mnemonic
* int32_t imm_i is the imm of the instruction
*
* @return a formatted I type alu instruction
**********************************************************/

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i){
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	std::ostringstream os;

    	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
    	return os.str();
}

/**
* This function formats an R type instruction
*
* @param uint32_t insn is a 32 bit instruction
* const char *mnemonic is a character array representing
* the instruction's mnemonic
*
* @return a string holding a formatted R type instruction
**********************************************************/

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic){
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	std::ostringstream os;

    	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    	return os.str();
}

/**
* This function formats an ecall instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a string holding a formatted ecall instruction
**********************************************************/

std::string rv32i_decode::render_ecall(uint32_t insn){
	std::ostringstream os;
    	os << "ecall";
    	return os.str();
}

/**
* This function formats an ebreak instruction
*
* @param uint32_t insn is a 32 bit instruction
*
* @return a string holding a formatted ebreak instruction
**********************************************************/

std::string rv32i_decode::render_ebreak(uint32_t insn){
	std::ostringstream os;
    	os << "ebreak";
    	return os.str();
}

/**
* This function formats a csrrx instruction
*
* @param uint32_t insn is a 32 bit instruction
* const char *mnemonic is a character array representing
* the instruction's mnemonic
*
* @return a string holding a formatted csrrx instruction
**********************************************************/

std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic){
	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t i = get_imm_i(insn);
    	std::ostringstream os;

    	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(i) << "," << render_reg(rs1);
    	return os.str();
}

/**
* This function formats a csrrxi instruction
*
* @param uint32_t insn is a 32 bit instruction
* const char *mnemonic is a character array representing
* the instruction's mnemonic
*
* @return a string holding a formatted csrrxi instruction
**********************************************************/

std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic){
	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t i = get_imm_i(insn);
    	std::ostringstream os;

    	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << hex::to_hex0x12(i) << "," << rs1;
    	return os.str();
}

/**
* This function formats a register's contents
*
* @param int r is the register
*
* @return a string representing the formatted register
* output
**********************************************************/

std::string rv32i_decode::render_reg(int r){
	std::ostringstream os;

    	os <<  "x" << r; //we must add x before printing the register
    	return os.str();
}

/**
* This function formats a register's contents after the
* displacement has been calculated
*
* @param uint32_t base is the base register
*        uint32_t disp is the displacement register
*
* @return a string representing the formatted output of a
* register that has been displaced
**********************************************************/

std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp){
	uint32_t diff = base - disp;
    	std::ostringstream os;

    	os << render_reg(diff);
    	return os.str();
}

/**
* This function formats a mnemonic
*
* @param string &m is the mnemonic that gets formatted
*
* @return the formatted mnemonic
**********************************************************/

std::string rv32i_decode::render_mnemonic(const std::string &m){
	std::ostringstream os;

    	os <<  std::setw(mnemonic_width) << std::left << m;
    	return os.str();
}

