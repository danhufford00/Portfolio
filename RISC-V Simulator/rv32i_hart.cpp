#include "rv32i_hart.h"

using namespace std;

/**
* The reset function resets the hart
***************************************************************************/

void rv32i_hart::reset(){
	pc = 0;
    	regs.reset();
    	insn_counter = 0;
    	halt = false;
    	halt_reason = "none";
}

/**
* The dump function dumps the entire state of the hart
*
* @param const std::string &hdr - a string that represents a header
***************************************************************************/

void rv32i_hart::dump(const std::string &hdr) const{
	regs.dump(hdr);
    	cout << " pc " << hex::to_hex32(pc) << endl;
}

/**
* The exec function executes a given RV32I instruction
*
* @param uint32_t insn - an unsigned 32 bit int representing the instruction
*    std::ostream* pos - an output stream pointer
***************************************************************************/

void rv32i_hart::exec(uint32_t insn, std::ostream* pos){
	if (insn == insn_ebreak){
        	exec_ebreak(insn, pos);
		return;
    	}

	switch(get_opcode(insn)){
        	case opcode_lui:     	exec_lui(insn, pos); return;
        	case opcode_auipc:   	exec_auipc(insn, pos); return;
        	case opcode_jal:     	exec_jal(insn, pos); return;
        	case opcode_jalr:    	exec_jalr(insn, pos); return;
        	case opcode_btype:
            		switch (get_funct3(insn)){ //funct3 decodes instructions with the same opcode
                		case funct3_beq:     	exec_beq(insn, pos); return;
                		case funct3_blt:     	exec_blt(insn, pos); return;
                		case funct3_bge:     	exec_bge(insn, pos); return;
                		case funct3_bne:     	exec_bne(insn, pos); return;
                		case funct3_bltu:    	exec_bltu(insn, pos); return;
                		case funct3_bgeu:    	exec_bgeu(insn, pos); return;
				default:		exec_illegal_insn(insn, pos); return;
            		}
            		assert(0 && "unrecognized opcode");
        	case opcode_load_imm:
            		switch (get_funct3(insn)){
                		case funct3_lh:      	exec_lh(insn, pos); return;
                		case funct3_lb:      	exec_lb(insn, pos); return;
                		case funct3_lw:      	exec_lw(insn, pos); return;
                		case funct3_lbu:     	exec_lbu(insn, pos); return;
                		case funct3_lhu:     	exec_lhu(insn, pos); return;
				default:		exec_illegal_insn(insn, pos); return;
            		}
        	case opcode_stype:
            		switch (get_funct3(insn)){
                		case funct3_sb:     	exec_sb(insn, pos); return;
                		case funct3_sh:     	exec_sh(insn, pos); return;
                		case funct3_sw:     	exec_sw(insn, pos); return;
				default:		exec_illegal_insn(insn, pos); return;
            		}
        	case opcode_alu_imm:
            		switch (get_funct3(insn)){
                		case funct3_add:    	exec_addi(insn, pos); return;
                		case funct3_sll:    	exec_slli(insn, pos); return;
                		case funct3_slt:    	exec_slti(insn, pos); return;
                		case funct3_sltu:   	exec_sltiu(insn, pos); return;
                		case funct3_xor:    	exec_xori(insn, pos); return;
                		case funct3_or:     	exec_ori(insn, pos); return;
                		case funct3_and:    	exec_andi(insn, pos); return;
                		case funct3_srx:
                    			switch(get_funct7(insn)){ //funct7 further decodes instructions with the same opcode
                        			case funct7_sra:	exec_srai(insn, pos); return;
                        			case funct7_srl:        exec_srli(insn, pos); return;
						default:		exec_illegal_insn(insn, pos); return;
                    			}
                    			assert(0 && "unrecognized funct7");
				default:		exec_illegal_insn(insn, pos); return;
            		}
        	case opcode_rtype:
            		switch(get_funct3(insn)){
                		case funct3_add:
                    			switch(get_funct7(insn)){
                        			case funct7_add:	exec_add(insn, pos); return;
                        			case funct7_sub:    	exec_sub(insn, pos); return;
                    			}
                		case funct3_srx:
                    			switch(get_funct7(insn)){
                        			case funct7_srl:    	exec_srl(insn, pos); return;
                        			case funct7_sra:    	exec_sra(insn, pos); return;
                    			}
                		case funct3_sll:    	exec_sll(insn, pos); return;
                		case funct3_slt:    	exec_slt(insn, pos); return;
                		case funct3_sltu:   	exec_sltu(insn, pos); return;
                		case funct3_xor:   	exec_xor(insn, pos); return;
                		case funct3_or:     	exec_or(insn, pos); return;
                		case funct3_and:    	exec_and(insn, pos); return;
				default:		exec_illegal_insn(insn, pos); return;
            		}
        	case opcode_system:
            		switch(get_funct3(insn)){
                		case funct3_csrrs:	exec_csrrs(insn, pos); return;
				default:		exec_illegal_insn(insn, pos); return;
        		}
		default:                exec_illegal_insn(insn, pos); return;
	}
}

/**
* The exec_illegal_insn function renders the proper error message by
* writing it to the pos output stream
*
* @param uint32_t insn - an unsigned 32 bit int representing an instruction
*    std::ostream *pos - an output stream pointer
***************************************************************************/

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream *pos){
	(void) insn;

    	if(pos)
        	*pos << render_illegal_insn (insn);

    	halt = true;
    	halt_reason = " Illegal instruction ";
}

/**
* The tick function tells the simulator to execute an instruction
*
* @param const std::string &hdr - a header that is printed to the left of
*				  all output displayed by this function
***************************************************************************/

void rv32i_hart::tick(const std::string &hdr){
	uint32_t insn = mem.get32(pc);
	insn_counter++;

    	if(show_registers)
        	dump(hdr);

    	if(show_instructions){
        	if(hdr.length() != 0)
            		cout << hdr << " " << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
        	else
            		cout << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
        	exec(insn, &std::cout);
    	}
	else
        	exec(insn, nullptr);
}

/**
* The exec_lui function executes the lui command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*			 instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos){
	uint32_t rd = get_rd(insn);
    	uint32_t immi_u = get_imm_u(insn);

    	if (pos){
        	std::string s = render_lui(insn);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std :: left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(immi_u) << endl;
    	}

    	regs.set(rd, immi_u);
    	pc += 4;
}

/**
* The exec_auipc function executes the auipc command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream * pos ){
    	uint32_t rd = get_rd(insn);
    	uint32_t immi_u = get_imm_u(insn);
    	int32_t val = pc + immi_u;

    	if (pos) {
        	std::string s = render_auipc(insn);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std :: left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(immi_u) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_jal function executes the jal command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t imm_j = get_imm_j(insn);
    	int32_t val = pc + imm_j;

    	if (pos){
        	std::string s = render_jal(pc, insn);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(pc+4) << ",  pc = " << hex::to_hex0x32(pc) << " + "  << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, pc+4);
    	pc += imm_j;
}

/**
* The exec_jalr function executes the jalr command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t val_rd = pc+4;
    	int32_t val_pc = (regs.get(rs1) + imm_i) & 0xfffffffe;

    	if (pos){
        	std::string s = render_jalr(insn);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(val_rd) << ",  pc = (" << hex::to_hex0x32(imm_i) << " + "  << hex::to_hex0x32(regs.get(rs1)) << ") & 0xfffffffe" << " = " << hex::to_hex0x32(val_pc) << endl;
    	}

    	regs.set(rd, val_rd);
    	pc = val_pc;
}

/**
* The exec_bne function executes the bne command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_b = get_imm_b(insn);
    	int32_t val = pc + ((regs.get(rs1) != regs.get(rs2)) ? imm_b : 4);

	if (pos){
        	std::string s = render_btype(pc, insn, "bne");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = "  << hex::to_hex0x32(val) << endl;
    	}

    	pc = val;
}

/**
* The exec_beq function executes the beq command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_b = get_imm_b(insn);
    	int32_t val = pc + ((regs.get(rs1) == regs.get(rs2)) ? imm_b : 4);

	if (pos){
        	std::string s = render_btype(pc, insn, "beq");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = "  << hex::to_hex0x32(val) << endl;
    	}

    	pc = val;
}

/**
* The exec_blt function executes the blt command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos){
    	int32_t rs1 = get_rs1(insn);
    	int32_t rs2 = get_rs2(insn);
    	int32_t imm_b = get_imm_b(insn);
    	int32_t val = pc + ((regs.get(rs1) < regs.get(rs2)) ? imm_b : 4);

    	if (pos){
        	std::string s = render_btype(pc, insn, "blt");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       		*pos << "/" << "/ pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = "  << hex::to_hex0x32(val) << endl;
    	}

    	pc = val;
}

/**
* The exec_bge function executes the bge command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos ){
    	int32_t rs1 = get_rs1(insn);
    	int32_t rs2 = get_rs2(insn);
    	int32_t imm_b = get_imm_b(insn);
    	int32_t val = pc + ((regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);

    	if (pos){
        	std::string s = render_btype(pc, insn, "bge");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = "  << hex::to_hex0x32(val) << endl;
    	}

    	pc = val;
}

/**
* The exec_bltu function executes the bltu command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_b = get_imm_b(insn);
    	uint32_t val = pc + ((static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2))) ? imm_b:4);

    	if (pos){
        	std::string s = render_btype(pc, insn, "bltu");
        	*pos << " " << std::setw(instruction_width) << std::setfill (' ') << std::left << s;
        	*pos << "/" << "/ pc += (" << hex::to_hex0x32 (regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = "  << hex::to_hex0x32(val) << endl;
    	}

    	pc = val;
}

/**
* The exec_bgeu function executes the bgeu command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_b = get_imm_b(insn);
    	int32_t val = pc + ((static_cast<uint32_t>(regs.get(rs1)) >= static_cast<uint32_t>(regs.get(rs2))) ? imm_b : 4);

    	if (pos){
        	std::string s = render_btype(pc, insn, "bgeu");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = "  << hex::to_hex0x32(val) << endl;
    	}

    	pc = val;
}

/**
* The exec_addi function executes the addi command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t val = regs.get(rs1) + imm_i;

    	if (pos){
        	std::string s = render_itype_alu(insn, "addi", imm_i);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << " = "  << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_lb function executes the lb command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t memaddr = regs.get(rs1) + imm_i;
    	int32_t val = mem.get8(memaddr);

    	if (val & 0x00000080)
        	val |= 0xffffff00;

    	if (pos){
        	std::string s = render_itype_load(insn, "lb");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_lbu function executes the lbu command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t memaddr = regs.get(rs1) + imm_i;
    	int32_t val = mem.get8(memaddr);

    	if (pos){
        	std::string s = render_itype_load(insn, "lbu");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
   	pc += 4;
}

/**
* The exec_lhu function executes the lhu command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t memaddr = regs.get(rs1) + imm_i;
    	uint32_t val = mem.get16(memaddr);

    	if (pos){
        	std::string s = render_itype_load(insn, "lhu");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_lh function executes the lh command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t memaddr = regs.get(rs1) + imm_i;
    	int32_t val = mem.get16(memaddr);

    	if (val & 0x00008000)
        	val |= 0xffff0000;

    	if (pos){
        	std::string s = render_itype_load(insn, "lh");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_lw function executes the lw command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos ){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t imm_i = get_imm_i(insn);
    	int32_t memaddr = regs.get(rs1) + imm_i;
    	int32_t val = mem.get32(memaddr);

    	if (val & 0x00000000)
        	val |= 0x00000000;

    	if (pos){
        	std::string s = render_itype_load(insn, "lw");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_sb function executes the sb command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos ){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_s = get_imm_s(insn);
    	int32_t memaddr = regs.get(rs1) + imm_s;
    	int32_t val = regs.get(rs2) & 0x000000ff;

    	if (pos){
        	std::string s = render_stype(insn, "sb");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(val) << endl;
    	}

    	mem.set8(memaddr, val);
    	pc += 4;
}

/**
* The exec_sh function executes the sh command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos ){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_s = get_imm_s(insn);
    	int32_t memaddr = regs.get(rs1) + imm_s;
    	int32_t val = regs.get(rs2) & 0x0000ffff;

    	if (pos){
        	std::string s = render_stype(insn, "sh");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(val) << endl;
    	}

    	mem.set16(memaddr, val);
    	pc += 4;
}

/**
* The exec_sw function executes the sw command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos ){
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t imm_s = get_imm_s(insn);
    	int32_t memaddr = regs.get(rs1) + imm_s;
    	int32_t val = regs.get(rs2);

    	if (pos){
        	std::string s = render_stype(insn, "sw");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(val) << endl;
    	}

   	mem.set32(memaddr, val);
    	pc += 4;
}

/**
* The exec_slti function executes the slti command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t imm_i = get_imm_i(insn);
    	int32_t val = (regs.get(rs1) < imm_i) ? 1 : 0;

    	if (pos){
        	std::string s = render_itype_alu(insn, "slti", imm_i);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_sltiu function executes the sltiu command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t imm_i = get_imm_i(insn);
    	int32_t val = ( static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(imm_i)) ? 1 : 0;

    	if (pos){
        	std::string s = render_itype_alu(insn, "sltiu", imm_i);
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_xori function executes the xori command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t imm_i = get_imm_i(insn);
    	int32_t val = regs.get(rs1) ^ imm_i;

	if (pos){
        	std::string s = render_itype_alu(insn, "xori", imm_i);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_ori function executes the ori command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t imm_i = get_imm_i(insn);
    	int32_t val = regs.get(rs1) | imm_i;

    	if (pos){
        	std::string s = render_itype_alu(insn, "ori", imm_i);
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_andi function executes the andi command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_andi(uint32_t insn, std :: ostream * pos) {
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t imm_i = get_imm_i(insn);
    	int32_t val = regs.get(rs1) & imm_i;

    	if (pos){
        	std::string s = render_itype_alu(insn, "andi", imm_i) ;
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_slli function executes the slli command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos) {
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t var_i = get_rs2(insn);
    	int32_t val = regs.get(rs1) << var_i;

    	if (pos){
        	std::string s = render_itype_alu(insn, "slli", var_i);
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << std::dec << var_i << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_srli function executes the srli command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t var_i;
    	int32_t mask;

    	mask = 0x80000000;
    	var_i = (get_rs2(insn)) & 0xfffffe1f;
    	mask = ~((mask >> var_i) << 1);
    	int32_t val = (regs.get(rs1) >> var_i) & mask;

    	if (pos){
        	std::string s = render_itype_alu(insn, "srli", var_i) ;
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec << var_i << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_srai function executes the srai command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t var_i = get_rs2(insn);
    	int32_t val = regs.get(rs1) >> var_i;

    	if (pos){
        	std::string s = render_itype_alu(insn, "srai", var_i);
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec << var_i << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_add function executes the add command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) + regs.get(rs2);

    	if (pos){
        	std::string s = render_rtype(insn, "add");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_sub function executes the sub command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) - regs.get(rs2);

    	if (pos){
        	std::string s = render_rtype(insn, "sub");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_sll function executes the sll command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) << (regs.get(rs2) % XLEN);

    	if (pos){
        	std::string s = render_rtype(insn, "sll");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << std::dec << (regs.get(rs2) % XLEN) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_slt function executes the slt command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

    	if (pos){
        	std::string s = render_rtype ( insn ,"slt") ;
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
   	pc += 4;
}

/**
* The exec_sltu function executes the sltu command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	uint32_t val = (static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2))) ? 1 : 0;

	if (pos){
        	std::string s = render_rtype(insn, "sltu");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_xor function executes the xor command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) ^ regs.get(rs2);

    	if (pos){
        	std::string s = render_rtype(insn, "xor");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_lui function executes the lui command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) >> (regs.get(rs2) % XLEN);

    	if (pos){
        	std::string s = render_rtype(insn, "sra");
        	*pos << " " <<std::setw(instruction_width) << std::setfill (' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec << (regs.get(rs2) % XLEN) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_srl function executes the srl command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t mask = 0x80000000;
    	int32_t rs2XLEN = regs.get(rs2) % XLEN;

    	mask = ~((mask >> rs2XLEN) << 1);
    	int32_t val = (regs.get(rs1) >> rs2XLEN) & mask;

    	if (pos){
        	std::string s = render_rtype(insn, "srl");
        	*pos << " " << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec << (regs.get(rs2) % XLEN) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_or function executes the or command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) | regs.get(rs2);

    	if (pos){
        	std::string s = render_rtype(insn, "or");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_and function executes the and command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	uint32_t rs2 = get_rs2(insn);
    	int32_t val = regs.get(rs1) & regs.get(rs2);

    	if (pos){
        	std::string s = render_rtype(insn, "and");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val) << endl;
    	}

    	regs.set(rd, val);
    	pc += 4;
}

/**
* The exec_csrrs function executes the csrrs command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos){
    	uint32_t rd = get_rd(insn);
    	uint32_t rs1 = get_rs1(insn);
    	int32_t csr = get_imm_i(insn) & 0x00000fff;

    	if (rs1 != 0 || csr != 0xf14){
        	halt = true;
        	halt_reason = "Illegal CSR in CSRSS instruction";
    	}

    	if (pos){
        	std::string s = render_csrrx(insn, "csrrs");
        	*pos << " " <<std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        	*pos << "/" << "/ " << render_reg(rd) << " = " << std::dec << mhartid << endl;
    	}

    	if(!halt){
    		regs.set(rd, mhartid);
    		pc += 4;
    	}
}

/**
* The exec_ebreak function executes the ebreak command
*
* @param uint32_t insn - an unsigned 32 bit integer representing an
*                        instruction
*    std::ostream* pos - A output stream pointer used to format output
***************************************************************************/

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos){
	if(pos){
        	std::string s = render_ebreak(insn);
        	*pos << " " << std::setw(instruction_width) << std::setfill (' ') << std::left << s;
        	*pos << "/" << "/ HALT" << endl;
    	}

    	halt = true ;
    	halt_reason = " EBREAK instruction";
}
