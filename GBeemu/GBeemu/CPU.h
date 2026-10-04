#pragma once
#include <cstdint>

#include "Instruction.h"
#include "TraceEntry.h"

class Emulator;
class Bus;

enum Flag
{
	FLAG_Z = 7,
	FLAG_N = 6,
	FLAG_H = 5,
	FLAG_C = 4
};

class CPU
{
public:
	CPU(Emulator& emulator, Bus& bus);

	void reset();

	//Execution
	void step();
	void tick(uint8_t cycles);
	void tickM(uint8_t cycles);
	void idle();

	//Memory Access
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	//Instruction Fetching
	uint8_t fetch8();
	uint16_t fetch16();

	const TraceEntry& getLastTrace() const;

	bool getFlag(Flag flag) const;
	void setFlag(Flag flag);
	void clearFlag(Flag flag);
	void setFlag(Flag flag, bool value);
	
private:
	Emulator& emulator;
	Bus& bus;

	//Registers
	uint8_t A;
	uint8_t F;
	
	uint8_t B;
	uint8_t C;
	
	uint8_t D;
	uint8_t E;

	uint8_t H;
	uint8_t L;

	uint16_t PC;
	uint16_t SP;

	//State
	bool ime;
	bool halted;
	bool stopped;

	TraceEntry lastTrace;

	void createTrace(
		uint16_t pc,
		uint8_t opcode,
		const Instruction& instruction
	);

	//Current instruction
	uint8_t opcode;

	void executeOpcode(uint8_t opcode);
	void executeCB(uint8_t opcode);

	//Instruction Tables
	Instruction instructions[0x100];
	Instruction cbInstructions[0x100];
	void buildInstructionTables();

	//Instruction Helpers
	void xor_a_value(uint8_t v);
	void bit_v_r(uint8_t v, uint8_t r);
	void inc_reg(uint8_t& r);

	//Instructions
	void NOP();
	//INC r
	void INC_A(); void INC_B();	void INC_C(); void INC_D();
	void INC_E(); void INC_H(); void INC_L();
	//DEC r
	//INC rr
	//DEC rr
	//AND r
	//OR r
	//XOR r
	void XOR_A(); void XOR_B(); void XOR_C(); void XOR_D();
	void XOR_E(); void XOR_H(); void XOR_L(); void XOR_HL();
	//SUB r
	//ADD A,r
	//ADD rr, rr
	//ADD rr, s8
	//LD r,r
	//LD r,d8
	//LD r,d8
	void LD_A_d8(); void LD_B_d8(); void LD_C_d8(); void LD_D_d8();
	void LD_E_d8(); void LD_H_d8(); void LD_L_d8();
	//LD r,(rr)
	void LD_A_HL(); void LD_A_BC(); void LD_A_DE();
	void LD_A_HL_M(); void LD_A_HL_P();
	void LD_B_HL(); void LD_C_HL(); void LD_D_HL();
	void LD_E_HL(); void LD_H_HL(); void LD_L_HL();
	//LD (rr),r
	void LD_HL_M_A(); void LD_HL_P_A();
	void LD_HL_A(); void LD_HL_B(); void LD_HL_C(); void LD_HL_D();
	void LD_HL_E(); void LD_HL_H(); void LD_HL_L(); void LD_HL_d8();
	void LD_BC_A();
	void LD_DE_A();
	//LD rr,d16
	void LD_BC_d16();
	void LD_DE_d16();
	void LD_HL_d16();
	void LD_SP_d16();
	//LD rr,rr
	//LD (addr),r
	void LD_aC_A(); void LD_a8_A(); void LD_a16_A(); void LD_a16_SP();
	//JP
	void JP_Z_a16(); void JP_C_a16();
	void JP_NZ_a16(); void JP_NC_a16();
	void JP_a16(); void JP_HL();
	//JR
	void JR_Z_s8(); void JR_C_s8();
	void JR_NZ_s8(); void JR_NC_s8();
	void JR_s8();
	//CALL
	void CALL_Z_a16(); void CALL_C_a16();
	void CALL_NZ_a16(); void CALL_NC_a16();
	void CALL_a16();
	//RET
	void RET(); void RETI();
	void RET_Z(); void RET_C();
	void RET_NZ(); void RET_NC();
	//BIT v,r
	void BIT_0_A(); void BIT_0_B(); void BIT_0_C(); void BIT_0_D();
	void BIT_0_E(); void BIT_0_H(); void BIT_0_L(); void BIT_0_HL();
	void BIT_1_A(); void BIT_1_B(); void BIT_1_C(); void BIT_1_D();
	void BIT_1_E(); void BIT_1_H(); void BIT_1_L(); void BIT_1_HL();
	void BIT_2_A(); void BIT_2_B(); void BIT_2_C(); void BIT_2_D();
	void BIT_2_E(); void BIT_2_H(); void BIT_2_L(); void BIT_2_HL();
	void BIT_3_A(); void BIT_3_B(); void BIT_3_C(); void BIT_3_D();
	void BIT_3_E(); void BIT_3_H(); void BIT_3_L(); void BIT_3_HL();
	void BIT_4_A(); void BIT_4_B(); void BIT_4_C(); void BIT_4_D();
	void BIT_4_E(); void BIT_4_H(); void BIT_4_L(); void BIT_4_HL();
	void BIT_5_A(); void BIT_5_B(); void BIT_5_C(); void BIT_5_D();
	void BIT_5_E(); void BIT_5_H(); void BIT_5_L(); void BIT_5_HL();
	void BIT_6_A(); void BIT_6_B(); void BIT_6_C(); void BIT_6_D();
	void BIT_6_E(); void BIT_6_H(); void BIT_6_L(); void BIT_6_HL();
	void BIT_7_A(); void BIT_7_B(); void BIT_7_C(); void BIT_7_D();
	void BIT_7_E(); void BIT_7_H(); void BIT_7_L(); void BIT_7_HL();
};