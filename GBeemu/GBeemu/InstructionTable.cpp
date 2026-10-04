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
	instructions[0x04] = { "INC B", &CPU::INC_B,  1, 4 };
	instructions[0x06] = { "LD B,d8", &CPU::LD_B_d8,  2, 8 };
	instructions[0x08] = { "LD (a16), SP", &CPU::LD_a16_SP,  3, 20 };
	instructions[0x0A] = { "LD A,(BC)", &CPU::LD_A_BC, 1, 8 };
	instructions[0x0C] = { "INC C", &CPU::INC_C,  1, 4 };
	instructions[0x0E] = { "LD C,d8", &CPU::LD_C_d8,  2, 8 };

	instructions[0x11] = { "LD DE,d16", &CPU::LD_DE_d16,  3, 12 };
	instructions[0x14] = { "INC D", &CPU::INC_D,  1, 4 };
	instructions[0x16] = { "LD D,d8", &CPU::LD_D_d8,  2, 8 };
	instructions[0x18] = { "JR s8", &CPU::JR_s8, 2, 12 };
	instructions[0x1A] = { "LD A,(DE)", &CPU::LD_A_DE, 1, 8 };
	instructions[0x1C] = { "INC E", &CPU::INC_E,  1, 4 };
	instructions[0x1E] = { "LD E,d8", &CPU::LD_E_d8,  2, 8 };

	instructions[0x20] = { "JR NZ,s8", &CPU::JR_NZ_s8, 2, 8 };
	instructions[0x21] = { "LD HL,d16", &CPU::LD_HL_d16,  3, 12 };
	instructions[0x22] = { "LD (HL+),A", &CPU::LD_HL_P_A,  1, 8 };
	instructions[0x24] = { "INC H", &CPU::INC_H,  1, 4 };
	instructions[0x26] = { "LD H,d8", &CPU::LD_H_d8,  2, 8 };
	instructions[0x28] = { "JR Z,s8", &CPU::JR_Z_s8, 2, 8 };
	instructions[0x2A] = { "LD A,(HL+)", &CPU::LD_A_HL_P, 1, 8 };
	instructions[0x2C] = { "INC L", &CPU::INC_L,  1, 4 };
	instructions[0x2E] = { "LD L,d8", &CPU::LD_L_d8,  2, 8 };

	instructions[0x30] = { "JR NC,s8", &CPU::JR_NC_s8, 2, 8 };
	instructions[0x31] = { "LD SP,d16", &CPU::LD_SP_d16, 3, 12 };
	instructions[0x32] = { "LD (HL-),A", &CPU::LD_HL_M_A,  1, 8 };
	instructions[0x38] = { "JR C,s8", &CPU::JR_C_s8, 2, 8 };
	instructions[0x3A] = { "LD A,(HL-)", &CPU::LD_A_HL_M, 1, 8 };
	instructions[0x3C] = { "INC A", &CPU::INC_A,  1, 4 };
	instructions[0x3E] = { "LD A,d8", &CPU::LD_A_d8,  2, 8 };

	instructions[0x46] = { "LD B,(HL)", &CPU::LD_B_HL, 1, 8 };
	instructions[0x4E] = { "LD C,(HL)", &CPU::LD_C_HL, 1, 8 };

	instructions[0x56] = { "LD D,(HL)", &CPU::LD_D_HL, 1, 8 };
	instructions[0x5E] = { "LD E,(HL)", &CPU::LD_E_HL, 1, 8 };

	instructions[0x66] = { "LD H,(HL)", &CPU::LD_H_HL, 1, 8 };
	instructions[0x6E] = { "LD L,(HL)", &CPU::LD_L_HL, 1, 8 };

	instructions[0x70] = { "LD (HL),B", &CPU::LD_HL_B,  1, 8 };
	instructions[0x71] = { "LD (HL),C", &CPU::LD_HL_C,  1, 8 };
	instructions[0x72] = { "LD (HL),D", &CPU::LD_HL_D,  1, 8 };
	instructions[0x73] = { "LD (HL),E", &CPU::LD_HL_E,  1, 8 };
	instructions[0x74] = { "LD (HL),H", &CPU::LD_HL_H,  1, 8 };
	instructions[0x75] = { "LD (HL),L", &CPU::LD_HL_L,  1, 8 };
	instructions[0x77] = { "LD (HL),A", &CPU::LD_HL_A,  1, 8 };
	instructions[0x7E] = { "LD A,(HL)", &CPU::LD_A_HL, 1, 8 };

	instructions[0xA8] = { "XOR B", &CPU::XOR_B,  1, 4 };
	instructions[0xA9] = { "XOR C", &CPU::XOR_C,  1, 4 };
	instructions[0xAA] = { "XOR D", &CPU::XOR_D,  1, 4 };
	instructions[0xAB] = { "XOR E", &CPU::XOR_E,  1, 4 };
	instructions[0xAC] = { "XOR H", &CPU::XOR_H,  1, 4 };
	instructions[0xAD] = { "XOR L", &CPU::XOR_L,  1, 4 };
	instructions[0xAE] = { "XOR (HL)", &CPU::XOR_HL,  1, 8 };
	instructions[0xAF] = { "XOR A", &CPU::XOR_A,  1, 4 };

	instructions[0xC2] = { "JP NZ,a16", &CPU::JP_NZ_a16, 3, 12 };
	instructions[0xC3] = { "JP a16", &CPU::JP_a16, 3, 16 };
	instructions[0xC4] = { "CALL NZ, a16", &CPU::CALL_NZ_a16, 3, 12 };
	instructions[0xCA] = { "JP Z,a16", &CPU::JP_Z_a16, 3, 12 };
	instructions[0xCC] = { "CALL Z, a16", &CPU::CALL_Z_a16, 3, 12 };
	instructions[0xCD] = { "CALL a16", &CPU::CALL_a16, 3, 24 };

	instructions[0xD2] = { "JP NC,a16", &CPU::JP_NC_a16, 3, 12 };
	instructions[0xD4] = { "CALL NC, a16", &CPU::CALL_NC_a16, 3, 12 };
	instructions[0xDA] = { "JP C,a16", &CPU::JP_C_a16, 3, 12 };
	instructions[0xDC] = { "CALL C, a16", &CPU::CALL_C_a16, 3, 12 };

	instructions[0xE0] = { "LD (a8),A", &CPU::LD_a8_A,  2, 12 };
	instructions[0xE2] = { "LD (C),A", &CPU::LD_aC_A,  1, 8 };
	instructions[0xE9] = { "JP HL", &CPU::JP_HL, 1, 4 };
	instructions[0xEA] = { "LD (a16),A", &CPU::LD_a16_A,  3, 16 };

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
//INC rr
//DEC rr
//AND r
//OR r
//XOR r
void CPU::XOR_A() {	uint8_t value = A; xor_a_value(value); }
void CPU::XOR_B() { uint8_t value = B; xor_a_value(value); }
void CPU::XOR_C() { uint8_t value = C; xor_a_value(value); }
void CPU::XOR_D() { uint8_t value = D; xor_a_value(value); }
void CPU::XOR_E() { uint8_t value = E; xor_a_value(value); }
void CPU::XOR_H() { uint8_t value = H; xor_a_value(value); }
void CPU::XOR_L() { uint8_t value = L; xor_a_value(value); }
void CPU::XOR_HL() { uint8_t value = read((H << 8) | L); tickM(1); xor_a_value(value); }
//SUB r
//ADD A,r
//ADD rr, rr
//ADD rr, s8
//LD r,r
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
void CPU::LD_HL_M_A() { uint16_t HL = (H << 8) | L; tickM(1); write(HL, A); HL--; H = HL >> 8; L = HL & 0xFF; }
void CPU::LD_HL_P_A() { uint16_t HL = (H << 8) | L; tickM(1); write(HL, A); HL++; H = HL >> 8; L = HL & 0xFF; }
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
// (addr),r
void CPU::LD_aC_A() { write(0xFF00 | C, A);	tickM(1); }
void CPU::LD_a8_A() { uint8_t value = fetch8(); tickM(1); write(0xFF00 | value, A); tickM(1); }
void CPU::LD_a16_A() { uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); write((high << 8) | low, A); tickM(1); }
void CPU::LD_a16_SP() {	uint8_t low = fetch8(); tickM(1); uint8_t high = fetch8(); tickM(1); uint16_t addr = (high << 8) | low; write(addr, SP & 0xFF); tickM(1); write(addr + 1, SP >> 8);	tickM(1); }
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
void CPU::RET() {

}



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