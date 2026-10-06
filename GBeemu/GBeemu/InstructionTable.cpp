#include "CPU.h"
#include "Instruction.h"

void CPU::buildInstructionTables()
{
	for (int i = 0; i < 0x100; i++)
	{
		instructions[i] = { "UNIMPL", nullptr, 0, 0 };
		cbInstructions[i] = { "CB-UNIMPL", nullptr, 0, 0 };
	}

	//0x00 - 0x0F
	instructions[0x00] = { "NOP", &CPU::NOP, 1, 4 };
	instructions[0x01] = { "LD BC,d16" , &CPU::LD_BC_d16, 3, 12 };
	instructions[0x02] = { "LD (BC),A", &CPU::LD_BC_A, 1, 8 };
	instructions[0x03] = { "INC BC", &CPU::INC_BC, 1, 8 };
	instructions[0x04] = { "INC B", &CPU::INC_B,  1, 4 };
	instructions[0x05] = { "DEC B", &CPU::DEC_B, 1, 4 };
	instructions[0x06] = { "LD B,d8", &CPU::LD_B_d8,  2, 8 };
	instructions[0x07] = { "RLCA", &CPU::RLCA, 1, 4 };
	instructions[0x08] = { "LD (a16), SP", &CPU::LD_a16_SP,  3, 20 };
	instructions[0x09] = { "ADD HL,BC", &CPU::ADD_HL_BC, 1, 8 };
	instructions[0x0A] = { "LD A,(BC)", &CPU::LD_A_BC, 1, 8 };
	instructions[0x0B] = { "DEC BC", &CPU::DEC_BC, 1, 8 };
	instructions[0x0C] = { "INC C", &CPU::INC_C,  1, 4 };
	instructions[0x0D] = { "DEC C", &CPU::DEC_C, 1, 4 };
	instructions[0x0E] = { "LD C,d8", &CPU::LD_C_d8,  2, 8 };
	instructions[0x0F] = { "RRCA", &CPU::RRCA, 1, 4 };

	instructions[0x11] = { "LD DE,d16", &CPU::LD_DE_d16,  3, 12 };
	instructions[0x12] = { "LD (DE),A", &CPU::LD_DE_A, 1, 8 };
	instructions[0x13] = { "INC DE", &CPU::INC_DE, 1, 8 };
	instructions[0x14] = { "INC D", &CPU::INC_D,  1, 4 };
	instructions[0x15] = { "DEC D", &CPU::DEC_D, 1, 4 };
	instructions[0x16] = { "LD D,d8", &CPU::LD_D_d8,  2, 8 };
	instructions[0x17] = { "RLA", &CPU::RLA, 1, 4 };
	instructions[0x18] = { "JR s8", &CPU::JR_s8, 2, 12 };
	instructions[0x19] = { "ADD HL,DE", &CPU::ADD_HL_DE, 1, 8 };
	instructions[0x1A] = { "LD A,(DE)", &CPU::LD_A_DE, 1, 8 };
	instructions[0x1B] = { "DEC DE", &CPU::DEC_DE, 1, 8 };
	instructions[0x1C] = { "INC E", &CPU::INC_E,  1, 4 };
	instructions[0x1E] = { "LD E,d8", &CPU::LD_E_d8,  2, 8 };
	instructions[0x1D] = { "DEC E", &CPU::DEC_E, 1, 4 };
	instructions[0x1F] = { "RRA", &CPU::RRA, 1, 4 };

	instructions[0x20] = { "JR NZ,s8", &CPU::JR_NZ_s8, 2, 8 };
	instructions[0x21] = { "LD HL,d16", &CPU::LD_HL_d16,  3, 12 };
	instructions[0x22] = { "LD (HL+),A", &CPU::LD_HL_P_A,  1, 8 };
	instructions[0x23] = { "INC HL", &CPU::INC_HL, 1, 8 };
	instructions[0x24] = { "INC H", &CPU::INC_H,  1, 4 };
	instructions[0x25] = { "DEC H", &CPU::DEC_H, 1, 4 };
	instructions[0x26] = { "LD H,d8", &CPU::LD_H_d8,  2, 8 };
	instructions[0x28] = { "JR Z,s8", &CPU::JR_Z_s8, 2, 8 };
	instructions[0x29] = { "ADD HL,HL", &CPU::ADD_HL_HL, 1, 8 };
	instructions[0x2A] = { "LD A,(HL+)", &CPU::LD_A_HL_P, 1, 8 };
	instructions[0x2B] = { "DEC HL", &CPU::DEC_HL, 1, 8 };
	instructions[0x2C] = { "INC L", &CPU::INC_L,  1, 4 };
	instructions[0x2D] = { "DEC L", &CPU::DEC_L, 1, 4 };
	instructions[0x2E] = { "LD L,d8", &CPU::LD_L_d8,  2, 8 };

	instructions[0x30] = { "JR NC,s8", &CPU::JR_NC_s8, 2, 8 };
	instructions[0x31] = { "LD SP,d16", &CPU::LD_SP_d16, 3, 12 };
	instructions[0x32] = { "LD (HL-),A", &CPU::LD_HL_M_A,  1, 8 };
	instructions[0x33] = { "INC SP", &CPU::INC_SP, 1, 8 };
	instructions[0x34] = { "INC (HL)", &CPU::INC_MEM_HL, 1, 12 };
	instructions[0x35] = { "DEC (HL)", &CPU::DEC_MEM_HL, 1, 12 };
	instructions[0x36] = { "LD (HL),d8", &CPU::LD_HL_d8, 2, 12 };
	instructions[0x38] = { "JR C,s8", &CPU::JR_C_s8, 2, 8 };
	instructions[0x39] = { "ADD HL,SP", &CPU::ADD_HL_SP, 1, 8 };
	instructions[0x3A] = { "LD A,(HL-)", &CPU::LD_A_HL_M, 1, 8 };
	instructions[0x3B] = { "DEC SP", &CPU::DEC_SP, 1, 8 };
	instructions[0x3C] = { "INC A", &CPU::INC_A,  1, 4 };
	instructions[0x3D] = { "DEC A", &CPU::DEC_A, 1, 4 };
	instructions[0x3E] = { "LD A,d8", &CPU::LD_A_d8,  2, 8 };

	instructions[0x40] = { "LD B,B", &CPU::LD_B_B, 1, 4 };
	instructions[0x41] = { "LD B,C", &CPU::LD_B_C, 1, 4 };
	instructions[0x42] = { "LD B,D", &CPU::LD_B_D, 1, 4 };
	instructions[0x43] = { "LD B,E", &CPU::LD_B_E, 1, 4 };
	instructions[0x44] = { "LD B,H", &CPU::LD_B_H, 1, 4 };
	instructions[0x45] = { "LD B,L", &CPU::LD_B_L, 1, 4 };
	instructions[0x46] = { "LD B,(HL)", &CPU::LD_B_HL, 1, 8 };
	instructions[0x47] = { "LD B,A", &CPU::LD_B_A, 1, 4 };
	instructions[0x48] = { "LD C,B", &CPU::LD_C_B, 1, 4 };
	instructions[0x49] = { "LD C,C", &CPU::LD_C_C, 1, 4 };
	instructions[0x4A] = { "LD C,D", &CPU::LD_C_D, 1, 4 };
	instructions[0x4B] = { "LD C,E", &CPU::LD_C_E, 1, 4 };
	instructions[0x4C] = { "LD C,H", &CPU::LD_C_H, 1, 4 };
	instructions[0x4D] = { "LD C,L", &CPU::LD_C_L, 1, 4 };
	instructions[0x4E] = { "LD C,(HL)", &CPU::LD_C_HL, 1, 8 };
	instructions[0x4F] = { "LD C,A", &CPU::LD_C_A, 1, 4 };

	instructions[0x50] = { "LD D,B", &CPU::LD_D_B, 1, 4 };
	instructions[0x51] = { "LD D,C", &CPU::LD_D_C, 1, 4 };
	instructions[0x52] = { "LD D,D", &CPU::LD_D_D, 1, 4 };
	instructions[0x53] = { "LD D,E", &CPU::LD_D_E, 1, 4 };
	instructions[0x54] = { "LD D,H", &CPU::LD_D_H, 1, 4 };
	instructions[0x55] = { "LD D,L", &CPU::LD_D_L, 1, 4 };
	instructions[0x56] = { "LD D,(HL)", &CPU::LD_D_HL, 1, 8 };
	instructions[0x57] = { "LD D,A", &CPU::LD_D_A, 1, 4 };
	instructions[0x58] = { "LD E,B", &CPU::LD_E_B, 1, 4 };
	instructions[0x59] = { "LD E,C", &CPU::LD_E_C, 1, 4 };
	instructions[0x5A] = { "LD E,D", &CPU::LD_E_D, 1, 4 };
	instructions[0x5B] = { "LD E,E", &CPU::LD_E_E, 1, 4 };
	instructions[0x5C] = { "LD E,H", &CPU::LD_E_H, 1, 4 };
	instructions[0x5D] = { "LD E,L", &CPU::LD_E_L, 1, 4 };
	instructions[0x5E] = { "LD E,(HL)", &CPU::LD_E_HL, 1, 8 };
	instructions[0x5F] = { "LD E,A", &CPU::LD_E_A, 1, 4 };

	instructions[0x60] = { "LD H,B", &CPU::LD_H_B, 1, 4 };
	instructions[0x61] = { "LD H,C", &CPU::LD_H_C, 1, 4 };
	instructions[0x62] = { "LD H,D", &CPU::LD_H_D, 1, 4 };
	instructions[0x63] = { "LD H,E", &CPU::LD_H_E, 1, 4 };
	instructions[0x64] = { "LD H,H", &CPU::LD_H_H, 1, 4 };
	instructions[0x65] = { "LD H,L", &CPU::LD_H_L, 1, 4 };
	instructions[0x66] = { "LD H,(HL)", &CPU::LD_H_HL, 1, 8 };
	instructions[0x67] = { "LD H,A", &CPU::LD_H_A, 1, 4 };
	instructions[0x68] = { "LD L,B", &CPU::LD_L_B, 1, 4 };
	instructions[0x69] = { "LD L,C", &CPU::LD_L_C, 1, 4 };
	instructions[0x6A] = { "LD L,D", &CPU::LD_L_D, 1, 4 };
	instructions[0x6B] = { "LD L,E", &CPU::LD_L_E, 1, 4 };
	instructions[0x6C] = { "LD L,H", &CPU::LD_L_H, 1, 4 };
	instructions[0x6D] = { "LD L,L", &CPU::LD_L_L, 1, 4 };
	instructions[0x6E] = { "LD L,(HL)", &CPU::LD_L_HL, 1, 8 };
	instructions[0x6F] = { "LD L,A", &CPU::LD_L_A, 1, 4 };

	instructions[0x70] = { "LD (HL),B", &CPU::LD_HL_B,  1, 8 };
	instructions[0x71] = { "LD (HL),C", &CPU::LD_HL_C,  1, 8 };
	instructions[0x72] = { "LD (HL),D", &CPU::LD_HL_D,  1, 8 };
	instructions[0x73] = { "LD (HL),E", &CPU::LD_HL_E,  1, 8 };
	instructions[0x74] = { "LD (HL),H", &CPU::LD_HL_H,  1, 8 };
	instructions[0x75] = { "LD (HL),L", &CPU::LD_HL_L,  1, 8 };
	instructions[0x77] = { "LD (HL),A", &CPU::LD_HL_A,  1, 8 };
	instructions[0x78] = { "LD A,B", &CPU::LD_A_B, 1, 4 };
	instructions[0x79] = { "LD A,C", &CPU::LD_A_C, 1, 4 };
	instructions[0x7A] = { "LD A,D", &CPU::LD_A_D, 1, 4 };
	instructions[0x7B] = { "LD A,E", &CPU::LD_A_E, 1, 4 };
	instructions[0x7C] = { "LD A,H", &CPU::LD_A_H, 1, 4 };
	instructions[0x7D] = { "LD A,L", &CPU::LD_A_L, 1, 4 };
	instructions[0x7E] = { "LD A,(HL)", &CPU::LD_A_HL, 1, 8 };
	instructions[0x7F] = { "LD A,A", &CPU::LD_A_A, 1, 4 };

	instructions[0x80] = { "ADD A,B", &CPU::ADD_A_B, 1, 4 };
	instructions[0x81] = { "ADD A,C", &CPU::ADD_A_C, 1, 4 };
	instructions[0x82] = { "ADD A,D", &CPU::ADD_A_D, 1, 4 };
	instructions[0x83] = { "ADD A,E", &CPU::ADD_A_E, 1, 4 };
	instructions[0x84] = { "ADD A,H", &CPU::ADD_A_H, 1, 4 };
	instructions[0x85] = { "ADD A,L", &CPU::ADD_A_L, 1, 4 };
	instructions[0x86] = { "ADD A,(HL)", &CPU::ADD_A_HL, 1, 8 };
	instructions[0x87] = { "ADD A,A", &CPU::ADD_A_A, 1, 4 };

	instructions[0x90] = { "SUB B", &CPU::SUB_B, 1, 4 };
	instructions[0x91] = { "SUB C", &CPU::SUB_C, 1, 4 };
	instructions[0x92] = { "SUB D", &CPU::SUB_D, 1, 4 };
	instructions[0x93] = { "SUB E", &CPU::SUB_E, 1, 4 };
	instructions[0x94] = { "SUB H", &CPU::SUB_H, 1, 4 };
	instructions[0x95] = { "SUB L", &CPU::SUB_L, 1, 4 };
	instructions[0x96] = { "SUB (HL)", &CPU::SUB_HL, 1, 8 };
	instructions[0x97] = { "SUB A", &CPU::SUB_A, 1, 4 };

	instructions[0xA0] = { "AND B", &CPU::AND_B, 1, 4 };
	instructions[0xA1] = { "AND C", &CPU::AND_C, 1, 4 };
	instructions[0xA2] = { "AND D", &CPU::AND_D, 1, 4 };
	instructions[0xA3] = { "AND E", &CPU::AND_E, 1, 4 };
	instructions[0xA4] = { "AND H", &CPU::AND_H, 1, 4 };
	instructions[0xA5] = { "AND L", &CPU::AND_L, 1, 4 };
	instructions[0xA6] = { "AND (HL)", &CPU::AND_HL, 1, 8 };
	instructions[0xA7] = { "AND A", &CPU::AND_A, 1, 4 };
	instructions[0xA8] = { "XOR B", &CPU::XOR_B,  1, 4 };
	instructions[0xA9] = { "XOR C", &CPU::XOR_C,  1, 4 };
	instructions[0xAA] = { "XOR D", &CPU::XOR_D,  1, 4 };
	instructions[0xAB] = { "XOR E", &CPU::XOR_E,  1, 4 };
	instructions[0xAC] = { "XOR H", &CPU::XOR_H,  1, 4 };
	instructions[0xAD] = { "XOR L", &CPU::XOR_L,  1, 4 };
	instructions[0xAE] = { "XOR (HL)", &CPU::XOR_HL,  1, 8 };
	instructions[0xAF] = { "XOR A", &CPU::XOR_A,  1, 4 };

	instructions[0xB0] = { "OR B", &CPU::OR_B, 1, 4 };
	instructions[0xB1] = { "OR C", &CPU::OR_C, 1, 4 };
	instructions[0xB2] = { "OR D", &CPU::OR_D, 1, 4 };
	instructions[0xB3] = { "OR E", &CPU::OR_E, 1, 4 };
	instructions[0xB4] = { "OR H", &CPU::OR_H, 1, 4 };
	instructions[0xB5] = { "OR L", &CPU::OR_L, 1, 4 };
	instructions[0xB6] = { "OR (HL)", &CPU::OR_HL, 1, 8 };
	instructions[0xB7] = { "OR A", &CPU::OR_A, 1, 4 };
	instructions[0xB8] = { "CP B", &CPU::CP_B, 1, 4 };
	instructions[0xB9] = { "CP C", &CPU::CP_C, 1, 4 };
	instructions[0xBA] = { "CP D", &CPU::CP_D, 1, 4 };
	instructions[0xBB] = { "CP E", &CPU::CP_E, 1, 4 };
	instructions[0xBC] = { "CP H", &CPU::CP_H, 1, 4 };
	instructions[0xBD] = { "CP L", &CPU::CP_L, 1, 4 };
	instructions[0xBE] = { "CP (HL)", &CPU::CP_HL, 1, 8 };
	instructions[0xBF] = { "CP A", &CPU::CP_A, 1, 4 };

	instructions[0xC0] = { "RET NZ", &CPU::RET_NZ, 1, 8 };
	instructions[0xC1] = { "POP BC", &CPU::POP_BC, 1, 12 };
	instructions[0xC2] = { "JP NZ,a16", &CPU::JP_NZ_a16, 3, 12 };
	instructions[0xC3] = { "JP a16", &CPU::JP_a16, 3, 16 };
	instructions[0xC4] = { "CALL NZ, a16", &CPU::CALL_NZ_a16, 3, 12 };
	instructions[0xC5] = { "PUSH BC", &CPU::PUSH_BC, 1, 16 };
	instructions[0xC6] = { "ADD A,d8", &CPU::ADD_A_d8, 2, 8 };
	instructions[0xC8] = { "RET Z", &CPU::RET_Z, 1, 8 };
	instructions[0xC9] = { "RET", &CPU::RET, 1, 16 };
	instructions[0xCA] = { "JP Z,a16", &CPU::JP_Z_a16, 3, 12 };
	instructions[0xCC] = { "CALL Z, a16", &CPU::CALL_Z_a16, 3, 12 };
	instructions[0xCD] = { "CALL a16", &CPU::CALL_a16, 3, 24 };

	instructions[0xD0] = { "RET NC", &CPU::RET_NC, 1, 8 };
	instructions[0xD1] = { "POP DE", &CPU::POP_DE, 1, 12 };
	instructions[0xD2] = { "JP NC,a16", &CPU::JP_NC_a16, 3, 12 };
	instructions[0xD4] = { "CALL NC, a16", &CPU::CALL_NC_a16, 3, 12 };
	instructions[0xD5] = { "PUSH DE", &CPU::PUSH_DE, 1, 16 };
	instructions[0xD6] = { "SUB d8", &CPU::SUB_d8, 2, 8 };
	instructions[0xD8] = { "RET C", &CPU::RET_C, 1, 8 };
	instructions[0xD9] = { "RETI", &CPU::RETI, 1, 16 };
	instructions[0xDA] = { "JP C,a16", &CPU::JP_C_a16, 3, 12 };
	instructions[0xDC] = { "CALL C, a16", &CPU::CALL_C_a16, 3, 12 };

	instructions[0xE0] = { "LD (a8),A", &CPU::LD_a8_A,  2, 12 };
	instructions[0xE1] = { "POP HL", &CPU::POP_HL, 1, 12 };
	instructions[0xE2] = { "LD (C),A", &CPU::LD_aC_A,  1, 8 };
	instructions[0xE5] = { "PUSH HL", &CPU::PUSH_HL, 1, 16 };
	instructions[0xE6] = { "AND d8", &CPU::AND_d8, 2, 8 };
	instructions[0xE8] = { "ADD SP,s8", &CPU::ADD_SP_s8, 2, 16 };
	instructions[0xE9] = { "JP HL", &CPU::JP_HL, 1, 4 };
	instructions[0xEA] = { "LD (a16),A", &CPU::LD_a16_A,  3, 16 };
	instructions[0xEE] = { "XOR d8", &CPU::XOR_d8,  2, 8 };

	instructions[0xF0] = { "LD A,(a8)", &CPU::LD_A_a8, 2, 12 };
	instructions[0xF1] = { "POP AF", &CPU::POP_AF, 1, 12 };
	instructions[0xF2] = { "LD A,(C)", &CPU::LD_A_aC, 1, 8 };
	instructions[0xF5] = { "PUSH AF", &CPU::PUSH_AF, 1, 16 };
	instructions[0xF6] = { "OR d8", &CPU::OR_d8, 2, 8 };
	instructions[0xF8] = { "LD HL,SP+s8", &CPU::LD_HL_SP_s8, 2, 12 };
	instructions[0xF9] = { "LD SP,HL", &CPU::LD_SP_HL, 1, 8 };
	instructions[0xFA] = { "LD A,(a16)", &CPU::LD_A_a16, 3, 16 };
	instructions[0xFE] = { "CP d8", &CPU::CP_d8, 2, 8 };

	//0x10 - 0x1F
	cbInstructions[0x10] = { "RL B", &CPU::RL_B, 2, 8 };
	cbInstructions[0x11] = { "RL C", &CPU::RL_C, 2, 8 };
	cbInstructions[0x12] = { "RL D", &CPU::RL_D, 2, 8 };
	cbInstructions[0x13] = { "RL E", &CPU::RL_E, 2, 8 };
	cbInstructions[0x14] = { "RL H", &CPU::RL_H, 2, 8 };
	cbInstructions[0x15] = { "RL L", &CPU::RL_L, 2, 8 };
	cbInstructions[0x16] = { "RL (HL)", &CPU::RL_HL, 2, 16 };
	cbInstructions[0x17] = { "RL A", &CPU::RL_A, 2, 8 };
	cbInstructions[0x18] = { "RR B", &CPU::RR_B, 2, 8 };
	cbInstructions[0x19] = { "RR C", &CPU::RR_C, 2, 8 };
	cbInstructions[0x1A] = { "RR D", &CPU::RR_D, 2, 8 };
	cbInstructions[0x1B] = { "RR E", &CPU::RR_E, 2, 8 };
	cbInstructions[0x1C] = { "RR H", &CPU::RR_H, 2, 8 };
	cbInstructions[0x1D] = { "RR L", &CPU::RR_L, 2, 8 };
	cbInstructions[0x1E] = { "RR (HL)", &CPU::RR_HL, 2, 16 };
	cbInstructions[0x1F] = { "RR A", &CPU::RR_A, 2, 8 };

	cbInstructions[0x20] = { "SLA B", &CPU::SLA_B, 2, 8 };
	cbInstructions[0x21] = { "SLA C", &CPU::SLA_C, 2, 8 };
	cbInstructions[0x22] = { "SLA D", &CPU::SLA_D, 2, 8 };
	cbInstructions[0x23] = { "SLA E", &CPU::SLA_E, 2, 8 };
	cbInstructions[0x24] = { "SLA H", &CPU::SLA_H, 2, 8 };
	cbInstructions[0x25] = { "SLA L", &CPU::SLA_L, 2, 8 };
	cbInstructions[0x26] = { "SLA (HL)", &CPU::SLA_HL, 2, 16 };
	cbInstructions[0x27] = { "SLA A", &CPU::SLA_A, 2, 8 };
	cbInstructions[0x28] = { "SRA B", &CPU::SRA_B, 2, 8 };
	cbInstructions[0x29] = { "SRA C", &CPU::SRA_C, 2, 8 };
	cbInstructions[0x2A] = { "SRA D", &CPU::SRA_D, 2, 8 };
	cbInstructions[0x2B] = { "SRA E", &CPU::SRA_E, 2, 8 };
	cbInstructions[0x2C] = { "SRA H", &CPU::SRA_H, 2, 8 };
	cbInstructions[0x2D] = { "SRA L", &CPU::SRA_L, 2, 8 };
	cbInstructions[0x2E] = { "SRA (HL)", &CPU::SRA_HL, 2, 16 };
	cbInstructions[0x2F] = { "SRA A", &CPU::SRA_A, 2, 8 };

	//0x40 - 0x4F
	cbInstructions[0x40] = { "BIT 0,B", &CPU::BIT_0_B, 2, 8 };
	cbInstructions[0x41] = { "BIT 0,C", &CPU::BIT_0_C, 2, 8 };
	cbInstructions[0x42] = { "BIT 0,D", &CPU::BIT_0_D, 2, 8 };
	cbInstructions[0x43] = { "BIT 0,E", &CPU::BIT_0_E, 2, 8 };
	cbInstructions[0x44] = { "BIT 0,H", &CPU::BIT_0_H, 2, 8 };
	cbInstructions[0x45] = { "BIT 0,L", &CPU::BIT_0_L, 2, 8 };
	cbInstructions[0x46] = { "BIT 0,HL", &CPU::BIT_0_HL, 2, 12 };
	cbInstructions[0x47] = { "BIT 0,A", &CPU::BIT_0_A, 2, 8 };
	cbInstructions[0x48] = { "BIT 1,B", &CPU::BIT_1_B, 2, 8 };
	cbInstructions[0x49] = { "BIT 1,C", &CPU::BIT_1_C, 2, 8 };
	cbInstructions[0x4A] = { "BIT 1,D", &CPU::BIT_1_D, 2, 8 };
	cbInstructions[0x4B] = { "BIT 1,E", &CPU::BIT_1_E, 2, 8 };
	cbInstructions[0x4C] = { "BIT 1,H", &CPU::BIT_1_H, 2, 8 };
	cbInstructions[0x4D] = { "BIT 1,L", &CPU::BIT_1_L, 2, 8 };
	cbInstructions[0x4E] = { "BIT 1,HL", &CPU::BIT_1_HL, 2, 12 };
	cbInstructions[0x4F] = { "BIT 1,A", &CPU::BIT_1_A, 2, 8 };

	//0x50 - 0x5F
	cbInstructions[0x50] = { "BIT 2,B", &CPU::BIT_2_B, 2, 8 };
	cbInstructions[0x51] = { "BIT 2,C", &CPU::BIT_2_C, 2, 8 };
	cbInstructions[0x52] = { "BIT 2,D", &CPU::BIT_2_D, 2, 8 };
	cbInstructions[0x53] = { "BIT 2,E", &CPU::BIT_2_E, 2, 8 };
	cbInstructions[0x54] = { "BIT 2,H", &CPU::BIT_2_H, 2, 8 };
	cbInstructions[0x55] = { "BIT 2,L", &CPU::BIT_2_L, 2, 8 };
	cbInstructions[0x56] = { "BIT 2,HL", &CPU::BIT_2_HL, 2, 12 };
	cbInstructions[0x57] = { "BIT 2,A", &CPU::BIT_2_A, 2, 8 };
	cbInstructions[0x58] = { "BIT 3,B", &CPU::BIT_3_B, 2, 8 };
	cbInstructions[0x59] = { "BIT 3,C", &CPU::BIT_3_C, 2, 8 };
	cbInstructions[0x5A] = { "BIT 3,D", &CPU::BIT_3_D, 2, 8 };
	cbInstructions[0x5B] = { "BIT 3,E", &CPU::BIT_3_E, 2, 8 };
	cbInstructions[0x5C] = { "BIT 3,H", &CPU::BIT_3_H, 2, 8 };
	cbInstructions[0x5D] = { "BIT 3,L", &CPU::BIT_3_L, 2, 8 };
	cbInstructions[0x5E] = { "BIT 3,HL", &CPU::BIT_3_HL, 2, 12 };
	cbInstructions[0x5F] = { "BIT 3,A", &CPU::BIT_3_A, 2, 8 };

	//0x60 - 0x6F
	cbInstructions[0x60] = { "BIT 4,B", &CPU::BIT_4_B, 2, 8 };
	cbInstructions[0x61] = { "BIT 4,C", &CPU::BIT_4_C, 2, 8 };
	cbInstructions[0x62] = { "BIT 4,D", &CPU::BIT_4_D, 2, 8 };
	cbInstructions[0x63] = { "BIT 4,E", &CPU::BIT_4_E, 2, 8 };
	cbInstructions[0x64] = { "BIT 4,H", &CPU::BIT_4_H, 2, 8 };
	cbInstructions[0x65] = { "BIT 4,L", &CPU::BIT_4_L, 2, 8 };
	cbInstructions[0x66] = { "BIT 4,HL", &CPU::BIT_4_HL, 2, 12 };
	cbInstructions[0x67] = { "BIT 4,A", &CPU::BIT_4_A, 2, 8 };
	cbInstructions[0x68] = { "BIT 5,B", &CPU::BIT_5_B, 2, 8 };
	cbInstructions[0x69] = { "BIT 5,C", &CPU::BIT_5_C, 2, 8 };
	cbInstructions[0x6A] = { "BIT 5,D", &CPU::BIT_5_D, 2, 8 };
	cbInstructions[0x6B] = { "BIT 5,E", &CPU::BIT_5_E, 2, 8 };
	cbInstructions[0x6C] = { "BIT 5,H", &CPU::BIT_5_H, 2, 8 };
	cbInstructions[0x6D] = { "BIT 5,L", &CPU::BIT_5_L, 2, 8 };
	cbInstructions[0x6E] = { "BIT 5,HL", &CPU::BIT_5_HL, 2, 12 };
	cbInstructions[0x6F] = { "BIT 5,A", &CPU::BIT_5_A, 2, 8 };

	//0x70 - 0x7F
	cbInstructions[0x70] = { "BIT 6,B", &CPU::BIT_6_B, 2, 8 };
	cbInstructions[0x71] = { "BIT 6,C", &CPU::BIT_6_C, 2, 8 };
	cbInstructions[0x72] = { "BIT 6,D", &CPU::BIT_6_D, 2, 8 };
	cbInstructions[0x73] = { "BIT 6,E", &CPU::BIT_6_E, 2, 8 };
	cbInstructions[0x74] = { "BIT 6,H", &CPU::BIT_6_H, 2, 8 };
	cbInstructions[0x75] = { "BIT 6,L", &CPU::BIT_6_L, 2, 8 };
	cbInstructions[0x76] = { "BIT 6,HL", &CPU::BIT_6_HL, 2, 12 };
	cbInstructions[0x77] = { "BIT 6,A", &CPU::BIT_6_A, 2, 8 };
	cbInstructions[0x78] = { "BIT 7,B", &CPU::BIT_7_B, 2, 8 };
	cbInstructions[0x79] = { "BIT 7,C", &CPU::BIT_7_C, 2, 8 };
	cbInstructions[0x7A] = { "BIT 7,D", &CPU::BIT_7_D, 2, 8 };
	cbInstructions[0x7B] = { "BIT 7,E", &CPU::BIT_7_E, 2, 8 };
	cbInstructions[0x7C] = { "BIT 7,H", &CPU::BIT_7_H, 2, 8 };
	cbInstructions[0x7D] = { "BIT 7,L", &CPU::BIT_7_L, 2, 8 };
	cbInstructions[0x7E] = { "BIT 7,HL", &CPU::BIT_7_HL, 2, 12 };
	cbInstructions[0x7F] = { "BIT 7,A", &CPU::BIT_7_A, 2, 8 };

	cbInstructions[0x80] = { "RES 0,B", &CPU::RES_0_B, 2, 8 };
	cbInstructions[0x81] = { "RES 0,C", &CPU::RES_0_C, 2, 8 };
	cbInstructions[0x82] = { "RES 0,D", &CPU::RES_0_D, 2, 8 };
	cbInstructions[0x83] = { "RES 0,E", &CPU::RES_0_E, 2, 8 };
	cbInstructions[0x84] = { "RES 0,H", &CPU::RES_0_H, 2, 8 };
	cbInstructions[0x85] = { "RES 0,L", &CPU::RES_0_L, 2, 8 };
	cbInstructions[0x86] = { "RES 0,(HL)", &CPU::RES_0_HL, 2, 16 };
	cbInstructions[0x87] = { "RES 0,A", &CPU::RES_0_A, 2, 8 };
	cbInstructions[0x88] = { "RES 1,B", &CPU::RES_1_B, 2, 8 };
	cbInstructions[0x89] = { "RES 1,C", &CPU::RES_1_C, 2, 8 };
	cbInstructions[0x8A] = { "RES 1,D", &CPU::RES_1_D, 2, 8 };
	cbInstructions[0x8B] = { "RES 1,E", &CPU::RES_1_E, 2, 8 };
	cbInstructions[0x8C] = { "RES 1,H", &CPU::RES_1_H, 2, 8 };
	cbInstructions[0x8D] = { "RES 1,L", &CPU::RES_1_L, 2, 8 };
	cbInstructions[0x8E] = { "RES 1,(HL)", &CPU::RES_1_HL, 2, 16 };
	cbInstructions[0x8F] = { "RES 1,A", &CPU::RES_1_A, 2, 8 };

	cbInstructions[0x90] = { "RES 2,B", &CPU::RES_2_B, 2, 8 };
	cbInstructions[0x91] = { "RES 2,C", &CPU::RES_2_C, 2, 8 };
	cbInstructions[0x92] = { "RES 2,D", &CPU::RES_2_D, 2, 8 };
	cbInstructions[0x93] = { "RES 2,E", &CPU::RES_2_E, 2, 8 };
	cbInstructions[0x94] = { "RES 2,H", &CPU::RES_2_H, 2, 8 };
	cbInstructions[0x95] = { "RES 2,L", &CPU::RES_2_L, 2, 8 };
	cbInstructions[0x96] = { "RES 2,(HL)", &CPU::RES_2_HL, 2, 16 };
	cbInstructions[0x97] = { "RES 2,A", &CPU::RES_2_A, 2, 8 };
	cbInstructions[0x98] = { "RES 3,B", &CPU::RES_3_B, 2, 8 };
	cbInstructions[0x99] = { "RES 3,C", &CPU::RES_3_C, 2, 8 };
	cbInstructions[0x9A] = { "RES 3,D", &CPU::RES_3_D, 2, 8 };
	cbInstructions[0x9B] = { "RES 3,E", &CPU::RES_3_E, 2, 8 };
	cbInstructions[0x9C] = { "RES 3,H", &CPU::RES_3_H, 2, 8 };
	cbInstructions[0x9D] = { "RES 3,L", &CPU::RES_3_L, 2, 8 };
	cbInstructions[0x9E] = { "RES 3,(HL)", &CPU::RES_3_HL, 2, 16 };
	cbInstructions[0x9F] = { "RES 3,A", &CPU::RES_3_A, 2, 8 };

	cbInstructions[0xA0] = { "RES 4,B", &CPU::RES_4_B, 2, 8 };
	cbInstructions[0xA1] = { "RES 4,C", &CPU::RES_4_C, 2, 8 };
	cbInstructions[0xA2] = { "RES 4,D", &CPU::RES_4_D, 2, 8 };
	cbInstructions[0xA3] = { "RES 4,E", &CPU::RES_4_E, 2, 8 };
	cbInstructions[0xA4] = { "RES 4,H", &CPU::RES_4_H, 2, 8 };
	cbInstructions[0xA5] = { "RES 4,L", &CPU::RES_4_L, 2, 8 };
	cbInstructions[0xA6] = { "RES 4,(HL)", &CPU::RES_4_HL, 2, 16 };
	cbInstructions[0xA7] = { "RES 4,A", &CPU::RES_4_A, 2, 8 };
	cbInstructions[0xA8] = { "RES 5,B", &CPU::RES_5_B, 2, 8 };
	cbInstructions[0xA9] = { "RES 5,C", &CPU::RES_5_C, 2, 8 };
	cbInstructions[0xAA] = { "RES 5,D", &CPU::RES_5_D, 2, 8 };
	cbInstructions[0xAB] = { "RES 5,E", &CPU::RES_5_E, 2, 8 };
	cbInstructions[0xAC] = { "RES 5,H", &CPU::RES_5_H, 2, 8 };
	cbInstructions[0xAD] = { "RES 5,L", &CPU::RES_5_L, 2, 8 };
	cbInstructions[0xAE] = { "RES 5,(HL)", &CPU::RES_5_HL, 2, 16 };
	cbInstructions[0xAF] = { "RES 5,A", &CPU::RES_5_A, 2, 8 };

	cbInstructions[0xB0] = { "RES 6,B", &CPU::RES_6_B, 2, 8 };
	cbInstructions[0xB1] = { "RES 6,C", &CPU::RES_6_C, 2, 8 };
	cbInstructions[0xB2] = { "RES 6,D", &CPU::RES_6_D, 2, 8 };
	cbInstructions[0xB3] = { "RES 6,E", &CPU::RES_6_E, 2, 8 };
	cbInstructions[0xB4] = { "RES 6,H", &CPU::RES_6_H, 2, 8 };
	cbInstructions[0xB5] = { "RES 6,L", &CPU::RES_6_L, 2, 8 };
	cbInstructions[0xB6] = { "RES 6,(HL)", &CPU::RES_6_HL, 2, 16 };
	cbInstructions[0xB7] = { "RES 6,A", &CPU::RES_6_A, 2, 8 };
	cbInstructions[0xB8] = { "RES 7,B", &CPU::RES_7_B, 2, 8 };
	cbInstructions[0xB9] = { "RES 7,C", &CPU::RES_7_C, 2, 8 };
	cbInstructions[0xBA] = { "RES 7,D", &CPU::RES_7_D, 2, 8 };
	cbInstructions[0xBB] = { "RES 7,E", &CPU::RES_7_E, 2, 8 };
	cbInstructions[0xBC] = { "RES 7,H", &CPU::RES_7_H, 2, 8 };
	cbInstructions[0xBD] = { "RES 7,L", &CPU::RES_7_L, 2, 8 };
	cbInstructions[0xBE] = { "RES 7,(HL)", &CPU::RES_7_HL, 2, 16 };
	cbInstructions[0xBF] = { "RES 7,A", &CPU::RES_7_A, 2, 8 };
}

//Helper Functions
void CPU::xor_a_value(uint8_t v)
{
	uint8_t result = A ^ v;
	A = result;
	setFlag(FLAG_Z, result == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	clearFlag(FLAG_C);
}

void CPU::or_a_value(uint8_t value)
{
	A |= value;

	setFlag(FLAG_Z, A == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	clearFlag(FLAG_C);
}

void CPU::sub_a_value(uint8_t v)
{
	uint8_t a = A;

	setFlag(FLAG_H, (a & 0x0F) < (v & 0x0F));
	setFlag(FLAG_C, a < v);

	uint8_t result = a - v;
	A = result;

	setFlag(FLAG_Z, result == 0);
	setFlag(FLAG_N);
}

void CPU::bit_v_r(uint8_t v, uint8_t r)
{
	setFlag(FLAG_Z, (r & (1 << v)) == 0);
	clearFlag(FLAG_N);
	setFlag(FLAG_H);
}

void CPU::inc_reg(uint8_t& r)
{
	uint8_t result = r + 1;

	setFlag(FLAG_Z, result == 0);
	clearFlag(FLAG_N);
	setFlag(FLAG_H, (r & 0x0F) == 0x0F);

	r = result;	
}

void CPU::dec_reg(uint8_t& r)
{
	uint8_t result = r - 1;

	setFlag(FLAG_Z, result == 0);
	setFlag(FLAG_N);
	setFlag(FLAG_H, (r & 0x0F) == 0x00);

	r = result;
}

void CPU::inc_rr(uint8_t& r1, uint8_t& r2)
{
	uint16_t rr = (r1 << 8) | r2;
	rr++;
	r1 = rr >> 8;
	r2 = rr & 0xFF;
}

void CPU::dec_rr(uint8_t& r1, uint8_t& r2)
{
	uint16_t rr = (r1 << 8) | r2;
	rr--;
	r1 = rr >> 8;
	r2 = rr & 0xFF;
}

void CPU::ld_reg_reg(uint8_t& dst, uint8_t v)
{
	dst = v;
}

void CPU::rl_r(uint8_t& r)
{
	bool carry = getFlag(FLAG_C);
	bool newCarry = (r & 0x80) != 0;

	r = (r << 1) | carry;

	setFlag(FLAG_Z, r == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	setFlag(FLAG_C, newCarry);
}

void CPU::rl_rr(uint8_t h, uint8_t l)
{
	uint16_t address = (h << 8) | l;

	uint8_t value = read(address);
	tickM(1);

	bool carry = getFlag(FLAG_C);
	bool newCarry = (value & 0x80) != 0;

	value = (value << 1) | carry;

	setFlag(FLAG_Z, value == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	setFlag(FLAG_C, newCarry);

	write(address, value);
	tickM(1);
}

void CPU::rr_r(uint8_t& r)
{
	bool carry = getFlag(FLAG_C);
	bool newCarry = (r & 0x01) != 0;

	r = (r >> 1) | (carry << 7);

	setFlag(FLAG_Z, r == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	setFlag(FLAG_C, newCarry);
}

void CPU::rr_rr(uint8_t h, uint8_t l)
{
	uint16_t address = (h << 8) | l;

	uint8_t value = read(address);
	tickM(1);

	bool carry = getFlag(FLAG_C);
	bool newCarry = (value & 0x01) != 0;

	value = (value >> 1) | (carry << 7);

	setFlag(FLAG_Z, value == 0);
	clearFlag(FLAG_N);
	setFlag(FLAG_H, false);
	setFlag(FLAG_C, newCarry);

	write(address, value);
	tickM(1);
}

void CPU::cp_value(uint8_t v)
{
	uint8_t a = A;
	uint16_t result = static_cast<uint16_t>(a) - static_cast<uint16_t>(v);

	setFlag(FLAG_Z, (result & 0xFF) == 0);
	setFlag(FLAG_N);
	setFlag(FLAG_H, (a & 0x0F) < (v & 0x0F));
	setFlag(FLAG_C, a < v);
}

void CPU::add_a_value(uint8_t value)
{
	uint16_t result =
		static_cast<uint16_t>(A) +
		static_cast<uint16_t>(value);

	setFlag(
		FLAG_H,
		((A & 0x0F) + (value & 0x0F)) > 0x0F
	);

	setFlag(
		FLAG_C,
		result > 0xFF
	);

	A = static_cast<uint8_t>(result);

	setFlag(FLAG_Z, A == 0);
	clearFlag(FLAG_N);
}

void CPU::add_hl_value(uint16_t value)
{
	uint16_t HL = (H << 8) | L;

	uint32_t result =
		static_cast<uint32_t>(HL) +
		static_cast<uint32_t>(value);

	setFlag(
		FLAG_H,
		((HL & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF
	);

	setFlag(
		FLAG_C,
		result > 0xFFFF
	);

	clearFlag(FLAG_N);

	// Z is unaffected

	HL = static_cast<uint16_t>(result);

	H = HL >> 8;
	L = HL & 0xFF;
}

void CPU::and_a_value(uint8_t value)
{
	A &= value;

	setFlag(FLAG_Z, A == 0);
	clearFlag(FLAG_N);
	setFlag(FLAG_H);
	clearFlag(FLAG_C);
}

void CPU::sla_r(uint8_t& reg)
{
	uint8_t old = reg;

	setFlag(FLAG_C, old & 0x80);

	reg = old << 1;

	setFlag(FLAG_Z, reg == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
}

void CPU::sla_hl()
{
	uint16_t address = (H << 8) | L;
	uint8_t value = read(address);

	tickM(1);

	setFlag(FLAG_C, value & 0x80);

	value <<= 1;

	write(address, value);

	tickM(1);

	setFlag(FLAG_Z, value == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
}

void CPU::sra_r(uint8_t& reg)
{
	uint8_t old = reg;

	setFlag(FLAG_C, old & 0x01);

	reg = (old >> 1) | (old & 0x80);

	setFlag(FLAG_Z, reg == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
}

void CPU::sra_hl()
{
	uint16_t address = (H << 8) | L;
	uint8_t value = read(address);

	tickM(1);

	setFlag(FLAG_C, value & 0x01);

	value = (value >> 1) | (value & 0x80);

	write(address, value);

	tickM(1);

	setFlag(FLAG_Z, value == 0);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
}

//Hardware Instructions
void CPU::NOP() { return; }
//INC r
void CPU::INC_A() { inc_reg(A); }
void CPU::INC_B() { inc_reg(B); }
void CPU::INC_C() { inc_reg(C); }
void CPU::INC_D() { inc_reg(D); }
void CPU::INC_E() { inc_reg(E); }
void CPU::INC_H() { inc_reg(H); }
void CPU::INC_L() { inc_reg(L); }
//DEC r
void CPU::DEC_A() { dec_reg(A); }
void CPU::DEC_B() { dec_reg(B); }
void CPU::DEC_C() { dec_reg(C); }
void CPU::DEC_D() { dec_reg(D); }
void CPU::DEC_E() { dec_reg(E); }
void CPU::DEC_H() { dec_reg(H); }
void CPU::DEC_L() { dec_reg(L); }
//INC rr
void CPU::INC_BC() { inc_rr(B, C); tickM(1); }
void CPU::INC_DE() { inc_rr(D, E); tickM(1); }
void CPU::INC_HL() { inc_rr(H, L); tickM(1); }
void CPU::INC_SP() { SP++; tickM(1); }
//INC (rr)
void CPU::INC_MEM_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); inc_reg(value); write(address, value); tickM(1); }
//DEC rr
void CPU::DEC_BC() { dec_rr(B, C); tickM(1); }
void CPU::DEC_DE() { dec_rr(D, E); tickM(1); }
void CPU::DEC_HL() { dec_rr(H, L); tickM(1); }
void CPU::DEC_SP() { SP--; tickM(1); }
//DEC (rr)
void CPU::DEC_MEM_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); dec_reg(value); write(address, value); tickM(1); }
//AND r
void CPU::AND_A() { and_a_value(A); }
void CPU::AND_B() { and_a_value(B); }
void CPU::AND_C() { and_a_value(C); }
void CPU::AND_D() { and_a_value(D); }
void CPU::AND_E() { and_a_value(E); }
void CPU::AND_H() { and_a_value(H); }
void CPU::AND_L() { and_a_value(L); }
void CPU::AND_d8() { uint8_t value = fetch8(); tickM(1); and_a_value(value); }
//AND (rr)
void CPU::AND_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); and_a_value(value); }
//OR r
void CPU::OR_A() { or_a_value(A); }
void CPU::OR_B() { or_a_value(B); }
void CPU::OR_C() { or_a_value(C); }
void CPU::OR_D() { or_a_value(D); }
void CPU::OR_E() { or_a_value(E); }
void CPU::OR_H() { or_a_value(H); }
void CPU::OR_L() { or_a_value(L); }
void CPU::OR_HL() { uint8_t value = read((H << 8) | L); tickM(1); or_a_value(value); }
void CPU::OR_d8() { uint8_t value = fetch8(); tickM(1); or_a_value(value); }
//XOR r
void CPU::XOR_A() {	uint8_t value = A; xor_a_value(value); }
void CPU::XOR_B() { uint8_t value = B; xor_a_value(value); }
void CPU::XOR_C() { uint8_t value = C; xor_a_value(value); }
void CPU::XOR_D() { uint8_t value = D; xor_a_value(value); }
void CPU::XOR_E() { uint8_t value = E; xor_a_value(value); }
void CPU::XOR_H() { uint8_t value = H; xor_a_value(value); }
void CPU::XOR_L() { uint8_t value = L; xor_a_value(value); }
void CPU::XOR_HL() { uint8_t value = read((H << 8) | L); tickM(1); xor_a_value(value); }
void CPU::XOR_d8() { uint8_t value = fetch8(); tickM(1); xor_a_value(value); }
//SUB r
void CPU::SUB_A() { uint8_t value = A; sub_a_value(value); }
void CPU::SUB_B() { uint8_t value = B; sub_a_value(value); }
void CPU::SUB_C() { uint8_t value = C; sub_a_value(value); }
void CPU::SUB_D() { uint8_t value = D; sub_a_value(value); }
void CPU::SUB_E() { uint8_t value = E; sub_a_value(value); }
void CPU::SUB_H() { uint8_t value = H; sub_a_value(value); }
void CPU::SUB_L() { uint8_t value = L; sub_a_value(value); }
void CPU::SUB_HL() { uint8_t value = read((H << 8) | L); tickM(1); sub_a_value(value); }
void CPU::SUB_d8() { uint8_t value = fetch8(); tickM(1); sub_a_value(value); }
//ADD A,r
void CPU::ADD_A_A() { add_a_value(A); }
void CPU::ADD_A_B() { add_a_value(B); }
void CPU::ADD_A_C() { add_a_value(C); }
void CPU::ADD_A_D() { add_a_value(D); }
void CPU::ADD_A_E() { add_a_value(E); }
void CPU::ADD_A_H() { add_a_value(H); }
void CPU::ADD_A_L() { add_a_value(L); }
void CPU::ADD_A_d8() { uint8_t value = fetch8(); tickM(1); add_a_value(value); }
//ADD A,(rr)
void CPU::ADD_A_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); add_a_value(value); }
//ADD rr, rr
void CPU::ADD_HL_BC() { uint16_t value = (B << 8) | C; tickM(1); add_hl_value(value); }
void CPU::ADD_HL_DE() { uint16_t value = (D << 8) | E; tickM(1); add_hl_value(value); }
void CPU::ADD_HL_HL() { uint16_t value = (H << 8) | L; tickM(1); add_hl_value(value); }
void CPU::ADD_HL_SP() { uint16_t value = SP; tickM(1); add_hl_value(value); }
//ADD rr, s8
void CPU::ADD_SP_s8()
{
	int8_t value =
		static_cast<int8_t>(fetch8());

	tickM(1);

	uint16_t unsignedValue =
		static_cast<uint8_t>(value);

	setFlag(
		FLAG_H,
		((SP & 0x0F) + (unsignedValue & 0x0F)) > 0x0F
	);

	setFlag(
		FLAG_C,
		((SP & 0xFF) + (unsignedValue & 0xFF)) > 0xFF
	);

	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);

	tickM(1);

	SP = static_cast<uint16_t>(
		SP + value
		);

	tickM(1);
}
//LD r,r
void CPU::LD_A_A() { ld_reg_reg(A, A); }
void CPU::LD_A_B() { ld_reg_reg(A, B); }
void CPU::LD_A_C() { ld_reg_reg(A, C); }
void CPU::LD_A_D() { ld_reg_reg(A, D); }
void CPU::LD_A_E() { ld_reg_reg(A, E); }
void CPU::LD_A_H() { ld_reg_reg(A, H); }
void CPU::LD_A_L() { ld_reg_reg(A, L); }
void CPU::LD_B_A() { ld_reg_reg(B, A); }
void CPU::LD_B_B() { ld_reg_reg(B, B); }
void CPU::LD_B_C() { ld_reg_reg(B, C); }
void CPU::LD_B_D() { ld_reg_reg(B, D); }
void CPU::LD_B_E() { ld_reg_reg(B, E); }
void CPU::LD_B_H() { ld_reg_reg(B, H); }
void CPU::LD_B_L() { ld_reg_reg(B, L); }
void CPU::LD_C_A() { ld_reg_reg(C, A); }
void CPU::LD_C_B() { ld_reg_reg(C, B); }
void CPU::LD_C_C() { ld_reg_reg(C, C); }
void CPU::LD_C_D() { ld_reg_reg(C, D); }
void CPU::LD_C_E() { ld_reg_reg(C, E); }
void CPU::LD_C_H() { ld_reg_reg(C, H); }
void CPU::LD_C_L() { ld_reg_reg(C, L); }
void CPU::LD_D_A() { ld_reg_reg(D, A); }
void CPU::LD_D_B() { ld_reg_reg(D, B); }
void CPU::LD_D_C() { ld_reg_reg(D, C); }
void CPU::LD_D_D() { ld_reg_reg(D, D); }
void CPU::LD_D_E() { ld_reg_reg(D, E); }
void CPU::LD_D_H() { ld_reg_reg(D, H); }
void CPU::LD_D_L() { ld_reg_reg(D, L); }
void CPU::LD_E_A() { ld_reg_reg(E, A); }
void CPU::LD_E_B() { ld_reg_reg(E, B); }
void CPU::LD_E_C() { ld_reg_reg(E, C); }
void CPU::LD_E_D() { ld_reg_reg(E, D); }
void CPU::LD_E_E() { ld_reg_reg(E, E); }
void CPU::LD_E_H() { ld_reg_reg(E, H); }
void CPU::LD_E_L() { ld_reg_reg(E, L); }
void CPU::LD_H_A() { ld_reg_reg(H, A); }
void CPU::LD_H_B() { ld_reg_reg(H, B); }
void CPU::LD_H_C() { ld_reg_reg(H, C); }
void CPU::LD_H_D() { ld_reg_reg(H, D); }
void CPU::LD_H_E() { ld_reg_reg(H, E); }
void CPU::LD_H_H() { ld_reg_reg(H, H); }
void CPU::LD_H_L() { ld_reg_reg(H, L); }
void CPU::LD_L_A() { ld_reg_reg(L, A); }
void CPU::LD_L_B() { ld_reg_reg(L, B); }
void CPU::LD_L_C() { ld_reg_reg(L, C); }
void CPU::LD_L_D() { ld_reg_reg(L, D); }
void CPU::LD_L_E() { ld_reg_reg(L, E); }
void CPU::LD_L_H() { ld_reg_reg(L, H); }
void CPU::LD_L_L() { ld_reg_reg(L, L); }
//LD r,d8
void CPU::LD_A_d8() { A = fetch8(); tickM(1); }
void CPU::LD_B_d8() { B = fetch8(); tickM(1); }
void CPU::LD_C_d8() { C = fetch8(); tickM(1); }
void CPU::LD_D_d8() { D = fetch8(); tickM(1); }
void CPU::LD_E_d8() { E = fetch8(); tickM(1); }
void CPU::LD_H_d8() { H = fetch8(); tickM(1); }
void CPU::LD_L_d8() { L = fetch8(); tickM(1); }
//LD r,(rr)
void CPU::LD_A_HL() { A = read((H << 8) | L); tickM(1); }
void CPU::LD_A_BC() { A = read((B << 8) | C); tickM(1); }
void CPU::LD_A_DE() { A = read((D << 8) | E); tickM(1); }
void CPU::LD_A_HL_M() {	uint16_t HL = (H << 8) | L;	A = read(HL); HL--;	H = HL >> 8; L = HL & 0xFF;	tickM(1); }
void CPU::LD_A_HL_P() {	uint16_t HL = (H << 8) | L;	A = read(HL); HL++;	H = HL >> 8; L = HL & 0xFF;	tickM(1); }
void CPU::LD_B_HL() { B = read((H << 8) | L); tickM(1); }
void CPU::LD_C_HL() { C = read((H << 8) | L); tickM(1); }
void CPU::LD_D_HL() { D = read((H << 8) | L); tickM(1); }
void CPU::LD_E_HL() { E = read((H << 8) | L); tickM(1); }
void CPU::LD_H_HL() { H = read((H << 8) | L); tickM(1); }
void CPU::LD_L_HL() { L = read((H << 8) | L); tickM(1); }
//LD (rr),r
void CPU::LD_HL_M_A() { uint16_t HL = (H << 8) | L; write(HL, A); tickM(1); HL--; H = HL >> 8; L = HL & 0xFF; }
void CPU::LD_HL_P_A() { uint16_t HL = (H << 8) | L; write(HL, A); tickM(1); HL++; H = HL >> 8; L = HL & 0xFF; }
void CPU::LD_HL_A() { write((H << 8) | L, A); tickM(1); }
void CPU::LD_HL_B() { write((H << 8) | L, B); tickM(1); }
void CPU::LD_HL_C() { write((H << 8) | L, C); tickM(1); }
void CPU::LD_HL_D() { write((H << 8) | L, D); tickM(1); }
void CPU::LD_HL_E() { write((H << 8) | L, E); tickM(1); }
void CPU::LD_HL_H() { write((H << 8) | L, H); tickM(1); }
void CPU::LD_HL_L() { write((H << 8) | L, L); tickM(1); }
void CPU::LD_HL_d8() { uint8_t value = fetch8(); tickM(1); write((H << 8) | L, value); tickM(1); }
void CPU::LD_BC_A() { uint16_t BC = (B << 8) | C; write(BC, A); tickM(1); }
void CPU::LD_DE_A() { uint16_t DE = (D << 8) | E; write(DE, A); tickM(1); }
//LD rr,d16
void CPU::LD_BC_d16() {	uint8_t tC = fetch8(); tickM(1); uint8_t tB = fetch8();	tickM(1); C = tC; B = tB; }
void CPU::LD_DE_d16() { uint8_t tE = fetch8(); tickM(1); uint8_t tD = fetch8();	tickM(1); E = tE; D = tD; }
void CPU::LD_HL_d16() { uint8_t tL = fetch8(); tickM(1); uint8_t tH = fetch8();	tickM(1); L = tL; H = tH; }
void CPU::LD_SP_d16() {	uint8_t low = fetch8();	tickM(1); uint8_t high = fetch8(); tickM(1); SP = low | (high << 8); }
//LD rr,rr
void CPU::LD_SP_HL() { SP = (H << 8) | L; tickM(1); }
void CPU::LD_HL_SP_s8()
{
	int8_t value =
		static_cast<int8_t>(fetch8());

	tickM(1);

	uint16_t unsignedValue =
		static_cast<uint8_t>(value);

	setFlag(
		FLAG_H,
		((SP & 0x0F) + (unsignedValue & 0x0F)) > 0x0F
	);

	setFlag(
		FLAG_C,
		((SP & 0xFF) + (unsignedValue & 0xFF)) > 0xFF
	);

	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);

	uint16_t result =
		static_cast<uint16_t>(SP + value);

	tickM(1);

	H = result >> 8;
	L = result & 0xFF;
}
// (addr),r
void CPU::LD_aC_A() { write(0xFF00 | C, A);	tickM(1); }
void CPU::LD_a8_A() { uint8_t value = fetch8(); tickM(1); write(0xFF00 | value, A); tickM(1); }
void CPU::LD_a16_A() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); write((high << 8) | low, A); tickM(1); }
void CPU::LD_a16_SP() {	uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); uint16_t addr = (high << 8) | low; write(addr, SP & 0xFF); tickM(1); write(addr + 1, SP >> 8);	tickM(1); }
// r,(addr)
void CPU::LD_A_a8() { uint8_t address = fetch8(); tickM(1); A = read(0xFF00 | address); tickM(1); }
void CPU::LD_A_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); A = read((high << 8) | low); tickM(1); }
void CPU::LD_A_aC() { A = read(0xFF00 | C);	tickM(1); }
//CP r
void CPU::CP_A() { cp_value(A); }
void CPU::CP_B() { cp_value(B); }
void CPU::CP_C() { cp_value(C); }
void CPU::CP_D() { cp_value(D); }
void CPU::CP_E() { cp_value(E); }
void CPU::CP_H() { cp_value(H); }
void CPU::CP_L() { cp_value(L); }
void CPU::CP_d8() { uint8_t value = fetch8(); tickM(1); cp_value(value); }
void CPU::CP_HL() {	uint8_t value = read((H << 8) | L); tickM(1); cp_value(value); }
//POP
void CPU::POP_AF() { uint8_t r1 = read(SP); SP++; tickM(1); uint8_t r2 = read(SP); SP++; tickM(1); A = r2; F = r1; F &= 0xF0; }
void CPU::POP_BC() { uint8_t r1 = read(SP); SP++; tickM(1); uint8_t r2 = read(SP); SP++; tickM(1); B = r2; C = r1; }
void CPU::POP_DE() { uint8_t r1 = read(SP); SP++; tickM(1); uint8_t r2 = read(SP); SP++; tickM(1); D = r2; E = r1; }
void CPU::POP_HL() { uint8_t r1 = read(SP); SP++; tickM(1); uint8_t r2 = read(SP); SP++; tickM(1); H = r2; L = r1; }
//PUSH
void CPU::PUSH_AF() { SP--; tickM(1); write(SP, A); SP--; tickM(1); write(SP, F); tickM(1); }
void CPU::PUSH_BC() { SP--; tickM(1); write(SP, B); SP--; tickM(1); write(SP, C); tickM(1); }
void CPU::PUSH_DE() { SP--; tickM(1); write(SP, D); SP--; tickM(1); write(SP, E); tickM(1); }
void CPU::PUSH_HL() { SP--; tickM(1); write(SP, H); SP--; tickM(1); write(SP, L); tickM(1); }
//JP
void CPU::JP_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); PC = (high << 8) | low; tickM(1); }
void CPU::JP_HL() {	PC = (H << 8) | L; }
void CPU::JP_Z_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (getFlag(FLAG_Z)) { PC = (high << 8) | low; tickM(1); } }
void CPU::JP_C_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (getFlag(FLAG_C)) { PC = (high << 8) | low; tickM(1); } }
void CPU::JP_NZ_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (!getFlag(FLAG_Z)) { PC = (high << 8) | low; tickM(1); } }
void CPU::JP_NC_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (!getFlag(FLAG_C)) { PC = (high << 8) | low; tickM(1); } }
//JR
void CPU::JR_s8() { int8_t i = static_cast<int8_t>(fetch8()); tickM(1); PC += i; tickM(1); }
void CPU::JR_Z_s8() { int8_t i = static_cast<int8_t>(fetch8()); tickM(1); if (getFlag(FLAG_Z)) { PC += i; tickM(1); } }
void CPU::JR_C_s8() { int8_t i = static_cast<int8_t>(fetch8()); tickM(1); if (getFlag(FLAG_C)) { PC += i; tickM(1); } }
void CPU::JR_NZ_s8() { int8_t i = static_cast<int8_t>(fetch8()); tickM(1); if (!getFlag(FLAG_Z)) { PC += i; tickM(1); } }
void CPU::JR_NC_s8() { int8_t i = static_cast<int8_t>(fetch8()); tickM(1); if (!getFlag(FLAG_C)) { PC += i; tickM(1); } }
//CALL
void CPU::CALL_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); SP--; tickM(1); write(SP, PC >> 8); SP--; tickM(1); write(SP, PC & 0xFF); PC = (high << 8) | low; tickM(1); }
void CPU::CALL_Z_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (getFlag(FLAG_Z)) { SP--; tickM(1); write(SP, PC >> 8); SP--; tickM(1); write(SP, PC & 0xFF); PC = (high << 8) | low; tickM(1); } }
void CPU::CALL_C_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (getFlag(FLAG_C)) { SP--; tickM(1); write(SP, PC >> 8); SP--; tickM(1); write(SP, PC & 0xFF); PC = (high << 8) | low; tickM(1); } }
void CPU::CALL_NZ_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (!getFlag(FLAG_Z)) { SP--; tickM(1); write(SP, PC >> 8); SP--; tickM(1); write(SP, PC & 0xFF); PC = (high << 8) | low; tickM(1); } }
void CPU::CALL_NC_a16() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); if (!getFlag(FLAG_C)) { SP--; tickM(1); write(SP, PC >> 8); SP--; tickM(1); write(SP, PC & 0xFF); PC = (high << 8) | low; tickM(1); } }
//RET
void CPU::RET() { uint8_t low = read(SP); SP++; tickM(1); uint8_t high = read(SP); SP++; tickM(1); PC = (high << 8) | low; tickM(1); }
void CPU::RET_Z() { tickM(1); if (getFlag(FLAG_Z)) { uint8_t low = read(SP); SP++; tickM(1); uint8_t high = read(SP); SP++; tickM(1); PC = (high << 8) | low; tickM(1);	} }
void CPU::RET_C() { tickM(1); if (getFlag(FLAG_C)) { uint8_t low = read(SP); SP++; tickM(1); uint8_t high = read(SP); SP++; tickM(1); PC = (high << 8) | low; tickM(1); } }
void CPU::RET_NZ() { tickM(1); if (!getFlag(FLAG_Z)) { uint8_t low = read(SP); SP++; tickM(1); uint8_t high = read(SP); SP++; tickM(1); PC = (high << 8) | low; tickM(1); } }
void CPU::RET_NC() { tickM(1); if (!getFlag(FLAG_C)) { uint8_t low = read(SP); SP++; tickM(1); uint8_t high = read(SP); SP++; tickM(1); PC = (high << 8) | low; tickM(1); } }
void CPU::RETI() { uint8_t low = read(SP); SP++; tickM(1); uint8_t high = read(SP); SP++; tickM(1); PC = (high << 8) | low; ime = true; tickM(1); }
//SLA r
void CPU::SLA_A() { sla_r(A); }
void CPU::SLA_B() { sla_r(B); }
void CPU::SLA_C() { sla_r(C); }
void CPU::SLA_D() { sla_r(D); }
void CPU::SLA_E() { sla_r(E); }
void CPU::SLA_H() { sla_r(H); }
void CPU::SLA_L() { sla_r(L); }
void CPU::SLA_HL() { sla_hl(); }
//SRA r
void CPU::SRA_A() { sra_r(A); }
void CPU::SRA_B() { sra_r(B); }
void CPU::SRA_C() { sra_r(C); }
void CPU::SRA_D() { sra_r(D); }
void CPU::SRA_E() { sra_r(E); }
void CPU::SRA_H() { sra_r(H); }
void CPU::SRA_L() { sra_r(L); }
void CPU::SRA_HL() { sra_hl(); }
//BIT v,r
void CPU::BIT_0_A() { bit_v_r(0, A); }
void CPU::BIT_0_B() { bit_v_r(0, B); }
void CPU::BIT_0_C() { bit_v_r(0, C); }
void CPU::BIT_0_D() { bit_v_r(0, D); }
void CPU::BIT_0_E() { bit_v_r(0, E); }
void CPU::BIT_0_H() { bit_v_r(0, H); }
void CPU::BIT_0_L() { bit_v_r(0, L); }
void CPU::BIT_0_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(0, value); }
void CPU::BIT_1_A() { bit_v_r(1, A); }
void CPU::BIT_1_B() { bit_v_r(1, B); }
void CPU::BIT_1_C() { bit_v_r(1, C); }
void CPU::BIT_1_D() { bit_v_r(1, D); }
void CPU::BIT_1_E() { bit_v_r(1, E); }
void CPU::BIT_1_H() { bit_v_r(1, H); }
void CPU::BIT_1_L() { bit_v_r(1, L); }
void CPU::BIT_1_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(1, value); }
void CPU::BIT_2_A() { bit_v_r(2, A); }
void CPU::BIT_2_B() { bit_v_r(2, B); }
void CPU::BIT_2_C() { bit_v_r(2, C); }
void CPU::BIT_2_D() { bit_v_r(2, D); }
void CPU::BIT_2_E() { bit_v_r(2, E); }
void CPU::BIT_2_H() { bit_v_r(2, H); }
void CPU::BIT_2_L() { bit_v_r(2, L); }
void CPU::BIT_2_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(2, value); }
void CPU::BIT_3_A() { bit_v_r(3, A); }
void CPU::BIT_3_B() { bit_v_r(3, B); }
void CPU::BIT_3_C() { bit_v_r(3, C); }
void CPU::BIT_3_D() { bit_v_r(3, D); }
void CPU::BIT_3_E() { bit_v_r(3, E); }
void CPU::BIT_3_H() { bit_v_r(3, H); }
void CPU::BIT_3_L() { bit_v_r(3, L); }
void CPU::BIT_3_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(3, value); }
void CPU::BIT_4_A() { bit_v_r(4, A); }
void CPU::BIT_4_B() { bit_v_r(4, B); }
void CPU::BIT_4_C() { bit_v_r(4, C); }
void CPU::BIT_4_D() { bit_v_r(4, D); }
void CPU::BIT_4_E() { bit_v_r(4, E); }
void CPU::BIT_4_H() { bit_v_r(4, H); }
void CPU::BIT_4_L() { bit_v_r(4, L); }
void CPU::BIT_4_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(4, value); }
void CPU::BIT_5_A() { bit_v_r(5, A); }
void CPU::BIT_5_B() { bit_v_r(5, B); }
void CPU::BIT_5_C() { bit_v_r(5, C); }
void CPU::BIT_5_D() { bit_v_r(5, D); }
void CPU::BIT_5_E() { bit_v_r(5, E); }
void CPU::BIT_5_H() { bit_v_r(5, H); }
void CPU::BIT_5_L() { bit_v_r(5, L); }
void CPU::BIT_5_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(5, value); }
void CPU::BIT_6_A() { bit_v_r(6, A); }
void CPU::BIT_6_B() { bit_v_r(6, B); }
void CPU::BIT_6_C() { bit_v_r(6, C); }
void CPU::BIT_6_D() { bit_v_r(6, D); }
void CPU::BIT_6_E() { bit_v_r(6, E); }
void CPU::BIT_6_H() { bit_v_r(6, H); }
void CPU::BIT_6_L() { bit_v_r(6, L); }
void CPU::BIT_6_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(6, value); }
void CPU::BIT_7_A() { bit_v_r(7, A); }
void CPU::BIT_7_B() { bit_v_r(7, B); }
void CPU::BIT_7_C() { bit_v_r(7, C); }
void CPU::BIT_7_D() { bit_v_r(7, D); }
void CPU::BIT_7_E() { bit_v_r(7, E); }
void CPU::BIT_7_H() { bit_v_r(7, H); }
void CPU::BIT_7_L() { bit_v_r(7, L); }
void CPU::BIT_7_HL() { uint16_t HL = (H << 8) | L; uint8_t value = read(HL); tickM(1); bit_v_r(7, value); }
//RES v,r
void CPU::RES_0_A() { A &= ~(1 << 0); }
void CPU::RES_0_B() { B &= ~(1 << 0); }
void CPU::RES_0_C() { C &= ~(1 << 0); }
void CPU::RES_0_D() { D &= ~(1 << 0); }
void CPU::RES_0_E() { E &= ~(1 << 0); }
void CPU::RES_0_H() { H &= ~(1 << 0); }
void CPU::RES_0_L() { L &= ~(1 << 0); }
void CPU::RES_0_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 0); write(address, value); tickM(1); }
void CPU::RES_1_A() { A &= ~(1 << 1); }
void CPU::RES_1_B() { B &= ~(1 << 1); }
void CPU::RES_1_C() { C &= ~(1 << 1); }
void CPU::RES_1_D() { D &= ~(1 << 1); }
void CPU::RES_1_E() { E &= ~(1 << 1); }
void CPU::RES_1_H() { H &= ~(1 << 1); }
void CPU::RES_1_L() { L &= ~(1 << 1); }
void CPU::RES_1_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 1); write(address, value); tickM(1); }
void CPU::RES_2_A() { A &= ~(1 << 2); }
void CPU::RES_2_B() { B &= ~(1 << 2); }
void CPU::RES_2_C() { C &= ~(1 << 2); }
void CPU::RES_2_D() { D &= ~(1 << 2); }
void CPU::RES_2_E() { E &= ~(1 << 2); }
void CPU::RES_2_H() { H &= ~(1 << 2); }
void CPU::RES_2_L() { L &= ~(1 << 2); }
void CPU::RES_2_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 2); write(address, value); tickM(1); }
void CPU::RES_3_A() { A &= ~(1 << 3); }
void CPU::RES_3_B() { B &= ~(1 << 3); }
void CPU::RES_3_C() { C &= ~(1 << 3); }
void CPU::RES_3_D() { D &= ~(1 << 3); }
void CPU::RES_3_E() { E &= ~(1 << 3); }
void CPU::RES_3_H() { H &= ~(1 << 3); }
void CPU::RES_3_L() { L &= ~(1 << 3); }
void CPU::RES_3_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 3); write(address, value); tickM(1); }
void CPU::RES_4_A() { A &= ~(1 << 4); }
void CPU::RES_4_B() { B &= ~(1 << 4); }
void CPU::RES_4_C() { C &= ~(1 << 4); }
void CPU::RES_4_D() { D &= ~(1 << 4); }
void CPU::RES_4_E() { E &= ~(1 << 4); }
void CPU::RES_4_H() { H &= ~(1 << 4); }
void CPU::RES_4_L() { L &= ~(1 << 4); }
void CPU::RES_4_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 4); write(address, value); tickM(1); }
void CPU::RES_5_A() { A &= ~(1 << 5); }
void CPU::RES_5_B() { B &= ~(1 << 5); }
void CPU::RES_5_C() { C &= ~(1 << 5); }
void CPU::RES_5_D() { D &= ~(1 << 5); }
void CPU::RES_5_E() { E &= ~(1 << 5); }
void CPU::RES_5_H() { H &= ~(1 << 5); }
void CPU::RES_5_L() { L &= ~(1 << 5); }
void CPU::RES_5_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 5); write(address, value); tickM(1); }
void CPU::RES_6_A() { A &= ~(1 << 6); }
void CPU::RES_6_B() { B &= ~(1 << 6); }
void CPU::RES_6_C() { C &= ~(1 << 6); }
void CPU::RES_6_D() { D &= ~(1 << 6); }
void CPU::RES_6_E() { E &= ~(1 << 6); }
void CPU::RES_6_H() { H &= ~(1 << 6); }
void CPU::RES_6_L() { L &= ~(1 << 6); }
void CPU::RES_6_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 6); write(address, value); tickM(1); }
void CPU::RES_7_A() { A &= ~(1 << 7); }
void CPU::RES_7_B() { B &= ~(1 << 7); }
void CPU::RES_7_C() { C &= ~(1 << 7); }
void CPU::RES_7_D() { D &= ~(1 << 7); }
void CPU::RES_7_E() { E &= ~(1 << 7); }
void CPU::RES_7_H() { H &= ~(1 << 7); }
void CPU::RES_7_L() { L &= ~(1 << 7); }
void CPU::RES_7_HL() { uint16_t address = (H << 8) | L; uint8_t value = read(address); tickM(1); value &= ~(1 << 7); write(address, value); tickM(1); }
//SET v,r
//RL r
void CPU::RL_A() { rl_r(A); }
void CPU::RL_B() { rl_r(B); }
void CPU::RL_C() { rl_r(C); }
void CPU::RL_D() { rl_r(D); }
void CPU::RL_E() { rl_r(E); }
void CPU::RL_H() { rl_r(H); }
void CPU::RL_L() { rl_r(L); }
void CPU::RL_HL() { rl_rr(H, L); }
//RR r
void CPU::RR_A() { rr_r(A); }
void CPU::RR_B() { rr_r(B); }
void CPU::RR_C() { rr_r(C); }
void CPU::RR_D() { rr_r(D); }
void CPU::RR_E() { rr_r(E); }
void CPU::RR_H() { rr_r(H); }
void CPU::RR_L() { rr_r(L); }
void CPU::RR_HL() { rr_rr(H, L); }
//RLCA, RLA, RRCA, RRA
void CPU::RLCA() { bool carry = (A & 0x80) != 0; A = (A << 1) | carry; clearFlag(FLAG_Z); clearFlag(FLAG_N); clearFlag(FLAG_H); setFlag(FLAG_C, carry); }
void CPU::RLA() { bool carry = (A & 0x80) != 0; bool oldCarry = getFlag(FLAG_C); A = (A << 1) | oldCarry; clearFlag(FLAG_Z); clearFlag(FLAG_N); clearFlag(FLAG_H); setFlag(FLAG_C, carry); }
void CPU::RRCA() { bool carry = (A & 0x01) != 0; A = (A >> 1) | (carry << 7); clearFlag(FLAG_Z); clearFlag(FLAG_N); clearFlag(FLAG_H); setFlag(FLAG_C, carry); }
void CPU::RRA() { bool carry = (A & 0x01) != 0; bool oldCarry = getFlag(FLAG_C); A = (A >> 1) | (oldCarry << 7); clearFlag(FLAG_Z); clearFlag(FLAG_N); clearFlag(FLAG_H); setFlag(FLAG_C, carry); }