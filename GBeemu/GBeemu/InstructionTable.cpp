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
	//instructions[0x02] = { &CPU::LD_BC_A, "LD (BC),A", 1, 8 };
	//instructions[0x03] = { &CPU::INC_BC, "INC BC", 1, 8 };
	//instructions[0x04] = { &CPU::INC_B, "INC B", 1, 4 };
	//instructions[0x05] = { &CPU::DEC_B, "DEC B", 1, 4 };
	//instructions[0x06] = { &CPU::LD_B, "LD B,d8", 2, 8 };
	//instructions[0x07] = { &CPU::RLCA, "RLCA", 1, 4 };
	//instructions[0x09] = { &CPU::ADD_HL_BC, "ADD HL,BC", 1, 8 };
	//instructions[0x08] = { &CPU::LD_a16_SP, "LD (a16), SP", 3, 20 };
	//instructions[0x0A] = { &CPU::LD_A_BC, "LD A,(BC)", 1, 8 };
	//instructions[0x0B] = { &CPU::DEC_BC, "DEC BC", 1, 8 };
	//instructions[0x0C] = { &CPU::INC_C, "INC C", 1, 4 };
	//instructions[0x0D] = { &CPU::DEC_C, "DEC C", 1, 4 };
	//instructions[0x0E] = { &CPU::LD_C, "LD C,d8", 2, 8 };
	//instructions[0x0F] = { &CPU::RRCA, "RRCA", 1, 4 };

	instructions[0x31] = { "LD SP,d16", &CPU::LD_SP_d16, 3, 12 };
}

//Hardware Instructions
void CPU::NOP() {  }
//INC r
//DEC r
//INC rr
//DEC rr
//AND r
//OR r
//XOR r
//SUB r
//ADD A,r
//ADD rr, rr
//ADD rr, s8
//LD r,r
//LD r,d8
//LD r,(rr)
//LD (rr),r
//LD rr,d16
void CPU::LD_BC_d16() {
	uint8_t tC = fetch8();
	tickM(1);
	uint8_t tB = fetch8();
	tickM(1);
	C = tC;
	B = tB;
}
void CPU::LD_SP_d16() {
	uint8_t low = fetch8();
	tickM(1);
	uint8_t high = fetch8();
	tickM(1);
	SP = low | (high << 8);
}