#include <cstdio>
#include <cstdint>

#define UNDEFINED_INST printf("Instruction Undefined\n")

void decode_itype(const uint32_t& inst, const uint8_t& opcode){
	auto imm   = (inst >> 20) & 0x0000'0FFF;
	auto rs1   = (inst >> 15) & 0b1'1111;
	auto func3 = (inst >> 12) & 0b111;
	auto rd    = (inst >> 7) & 0b1'1111;

	// Extract shift fields
	auto shamt   = imm & 0b11'1111;
	auto shiftop = (imm >> 6) & 0b11'1111;
	// Extract word shift fields
	auto shamtw   = imm & 0b1'1111;
	auto shiftopw = (imm >> 5) & 0b111'1111;

	// Decode Instruction
	switch(opcode) {
		case 0b0000011:
			switch(func3) {
				case 0b000: /*LB*/
					printf("LB X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b001: /*LH*/
					printf("LH X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b010: /*LW*/
					printf("LW X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b100: /*LBU*/
					printf("LBU X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b101: /*LHU*/
					printf("LHU X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b110: /*LWU*/
					printf("LWU X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b011: /*LD*/
					printf("LD { rd: X%d, rs1: X%d, imm: %d }\n", rd, rs1, imm);
					break;
				default:
					UNDEFINED_INST;
			}
			break;
		case 0b0001111:
			if(func3 == 0b000) {
				auto succ = imm & 0b1111;
				auto pred = (imm >> 4) & 0b1111;
				auto fm   = (imm >> 8) & 0b1111;
				printf("FENCE { rd: %d, rs1: %d, succ: %d, pred: %d, fm: %d }\n", rd, rs1, succ, pred, fm);
			} else UNDEFINED_INST;
			break;
		case 0b0010011:
			switch(func3) {
				case 0b000: /*ADDI*/
					printf("ADDI X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b010: /*SLTI*/
					printf("SLTI X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b011: /*SLTIU*/
					printf("SLTIU X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b100: /*XORI*/
					printf("XORI X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b110: /*ORI*/
					printf("ORI X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b111: /*ANDI*/
					printf("ANDI X%d, X%d, %d\n", rd, rs1, imm);
					break;
				case 0b001: /*SLLI*/
					if (shiftop == 0)
						printf("SLLI X%d, X%d, %d\n", rd, rs1, shamt);
					else UNDEFINED_INST;
					break;
				case 0b101: /*SRLI*/
					if (shiftop == 0)
						printf("SRLI X%d, X%d, %d\n", rd, rs1, shamt);
					else if(shiftop == 0b01'0000) /*SRAI*/
						printf("SRAI X%d, X%d, %d\n", rd, rs1, shamt);
					else UNDEFINED_INST;
					break;
				default:
					UNDEFINED_INST;
			}
			break;
		case 0b0011011:
			switch(func3) {
				case 0b000: /*ADDIW*/
					printf("ADDIW { rd: %d, rs1: %d, imm: %d}\n", rd, rs1, imm);
					break;
				case 0b001: /*SLLIW*/
					if(shiftopw == 0)
						printf("SLLIW {rd: %d, rs1: %d, shamt: %d}\n",
								rd, rs1, shamtw);
					else UNDEFINED_INST;
					break;
				case 0b101:
					if(shiftopw == 0) /*SRLIW*/
						printf("SRLIW {rd: %d, rs1: %d, shamt: %d}\n",
								rd, rs1, shamtw);
					else if(shiftopw == 0b0100000) /*SRAIW*/
						printf("SRAIW {rd: %d, rs1: %d, shamt: %d}\n",
								rd, rs1, shamtw);
					else UNDEFINED_INST;
					break;
				default:
					UNDEFINED_INST;
			}
			break;
		case 0b1100111:
			if(func3 == 0b000)
				printf("JALR {rd: %d, rs1: %d, imm: %d }\n", rd, rs1, imm);
			else UNDEFINED_INST;
			break;
		case 0b1110011:
			if(func3 == 0 && rd == 0 && rs1 == 0) {
				if(imm == 0) printf("ECALL\n");
				else if(imm == 1) printf("EBREAK\n");
			} else UNDEFINED_INST;
			break;
		default:
			UNDEFINED_INST;
	}
}

void decode_utype(const uint32_t& inst, const uint8_t& opcode){
	auto rd  = (inst >> 7) & 0b1'1111;
	auto imm = inst & 0xFFFFF000;

	switch(opcode) {
		case 0b0010111: /*AUIPC*/
			printf("AUIPC {rd: %d, imm: %d}\n", rd, imm);
			break;
		case 0b0110111: /*LUI*/
			printf("LUI {rd: %d, imm: %d}\n", rd, imm);
			break;
		default:
			UNDEFINED_INST;
	}
}

void decode_stype(const uint32_t& inst, const uint8_t& opcode){
	auto func3  = (inst >> 12) & 0b111;
	auto imm40  = (inst >> 7)  & 0b1'1111;
	auto rs1    = (inst >> 15) & 0b1'1111;
	auto rs2    = (inst >> 20) & 0b1'1111;
	auto imm115 = (inst >> 25) & 0b111'1111;
	auto imm    = (imm115 << 5) | imm40;

	switch(func3) {
		case 0b000: /*SB*/
			printf("SB { rs1: %d, rs2: %d, imm: %d}\n", rs1, rs2, imm);
			break;
		case 0b001: /*SH*/
			printf("SH { rs1: %d, rs2: %d, imm: %d}\n", rs1, rs2, imm);
			break;
		case 0b010: /*SW*/
			printf("SW { rs1: %d, rs2: %d, imm: %d}\n", rs1, rs2, imm);
			break;
		case 0b011: /*SD*/
			printf("SD { rs1: %d, rs2: %d, imm: %d}\n", rs1, rs2, imm);
			break;
		default:
			UNDEFINED_INST;
	}
}

void decode_rtype(const uint32_t& inst, const uint8_t& opcode){
	auto func3 = (inst >> 12) & 0b111;
	auto rd    = (inst >> 7)  & 0b1'1111;
	auto rs1   = (inst >> 15) & 0b1'1111;
	auto rs2   = (inst >> 20) & 0b1'1111;
	auto func7 = (inst >> 25) & 0b111'1111;

	switch(opcode) {
		case 0b0110011:
			switch(func3) {
				case 0b000:
					if(func7 == 0) /*ADD*/
						printf("ADD { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else if(func7 == 0b0100000) /*SUB*/
						printf("SUB { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else UNDEFINED_INST;
					break;
				case 0b001: /*SLL*/
					if(func7 == 0)
						printf("SLL { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else
						UNDEFINED_INST;
					break;
				case 0b010: /*SLT*/
					if(func7 == 0)
						printf("SLT { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else
						UNDEFINED_INST;
					break;
				case 0b011: /*SLTU*/
					if(func7 == 0)
						printf("SLTU { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else
						UNDEFINED_INST;
					break;
				case 0b100: /*XOR*/
					if(func7 == 0)
						printf("XOR { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else
						UNDEFINED_INST;
					break;
				case 0b101:
					if(func7 == 0) { /*SRL*/
						printf("SRL { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					} else if(func7 == 0b0100000) { /*SRA*/
						printf("SRA { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					} else UNDEFINED_INST;
					break;
				case 0b110: /*OR*/
					if(func7 == 0)
						printf("OR { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else
						UNDEFINED_INST;
					break;
				case 0b111: /*AND*/
					if(func7 == 0)
						printf("AND { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else
						UNDEFINED_INST;
					break;
				default: UNDEFINED_INST;
			}
			break;
		case 0b0111011:
			switch(func3){
				case 0b000:
					if(func7 == 0) /*ADDW*/
						printf("ADDW { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else if(func7 == 0b0100000) /*SUBW*/
						printf("SUBW { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else UNDEFINED_INST;
					break;
				case 0b001:
					if(func7 == 0) /*SLLW*/
						printf("SLLW {rd: %d, rs1: %d, rs2: %d }\n",
								rd, rs1, rs2);
					else UNDEFINED_INST;
					break;
				case 0b101:
					if(func7 == 0) /*SRLW*/
						printf("SRLW {rd: %d, rs1: %d, rs2: %d }\n",
								rd, rs1, rs2);
					else if(func7 == 0b0100000) /*SRAW*/
						printf("SRAW { rd: %d, rs1: %d, rs2: %d }\n", rd, rs1, rs2);
					else UNDEFINED_INST;
					break;
				default: UNDEFINED_INST;
			}
			break;
		default: UNDEFINED_INST;
	}

}

void decode_btype(const uint32_t& inst, const uint8_t& opcode){
	auto func3  = (inst >> 12) & 0b111;
	auto rs1    = (inst >> 15) & 0b1'1111;
	auto rs2    = (inst >> 20) & 0b1'1111;

	auto imm12105 = (inst >> 25) & 0b111'1111;
	auto imm4111  = (inst >> 7)  & 0b1'1111;

	auto imm41  = (imm4111 & 0b1'1110) >> 1;
	auto imm11  = imm4111 & 0b0'0001;
	auto imm12  = (imm12105 & 0b1000000) >> 6;
	auto imm105 = imm12105 & 0b0111111;
	auto imm = (imm12 << 12) | (imm11 << 11) | (imm105 << 5) | (imm41 << 1);

	switch(func3) {
		case 0b000: /*BEQ*/
			printf("BEQ { rs1: %d, rs2: %d}\n", rs1, rs2);
			break;
		case 0b001: /*BNE*/
			printf("BNE { rs1: %d, rs2: %d, imm: %d}\n", rs1, rs2, imm);
			break;
		case 0b100: /*BLT*/
			printf("BLT { rs1: %d, rs2: %d}\n", rs1, rs2);
			break;
		case 0b101: /*BGE*/
			printf("BGE { rs1: %d, rs2: %d}\n", rs1, rs2);
			break;
		case 0b110: /*BLTU*/
			printf("BLTU { rs1: %d, rs2: %d}\n", rs1, rs2);
			break;
		case 0b111: /*BGEU*/
			printf("BGEU { rs1: %d, rs2: %d}\n", rs1, rs2);
			break;
		default:
			UNDEFINED_INST;
	}
}

void decode_jtype(const uint32_t& inst, const uint8_t& opcode){
	auto rd  = (inst >> 7)  & 0b1'1111;
	auto imm = (inst & 0xFFFFF000) >> 12;

	auto imm20   = (imm >> 19) & 1;
	auto imm101  = (imm >> 9) & 0b11'1111'1111;
	auto imm11   = (imm >> 8) & 1;
	auto imm1912 = imm & 0b1111'1111;

	imm = (imm20 << 20) | (imm1912 << 12) | (imm11 << 11) | (imm101 << 1);
	printf("JAL { rd: %d, imm: %d}\n", rd, imm);
}

void decode_inst(const uint32_t& inst){
	auto opcode = inst & 0b111'1111;

	// Decode Type
	switch(opcode) {
		/* I-Type */
		case 0b0000011:
		case 0b0001111:
		case 0b0010011:
		case 0b0011011:
		case 0b1100111:
		case 0b1110011:
			decode_itype(inst, opcode);
			break;
		/* U-Type */
		case 0b0010111:
		case 0b0110111:
			decode_utype(inst, opcode);
			break;
		/* S-Type */
		case 0b0100011:
			decode_stype(inst, opcode);
			break;
		/* R-Type */
		case 0b0110011:
		case 0b0111011:
			decode_rtype(inst, opcode);
			break;
		/* B-Type */
		case 0b1100011:
			decode_btype(inst, opcode);
			break;
		/* J-Type */
		case 0b1101111:
			decode_jtype(inst, opcode);
			break;
		default:
			UNDEFINED_INST;
	}
}

int main(int argc, char** argv) {
	decode_inst(0x0188d89b);
	return 0;
}
