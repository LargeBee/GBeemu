#pragma once
#include <cstdint>

#include <fstream>
#include <iomanip>

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

	uint64_t getInstructionCount() const;
	
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
	std::ofstream traceFile;

	uint64_t instructionCount = 0;

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
	void sub_a_value(uint8_t v);
	void bit_v_r(uint8_t v, uint8_t r);
	void inc_reg(uint8_t& r);
	void dec_reg(uint8_t& r);
	void inc_rr(uint8_t& h, uint8_t& l);
	void dec_rr(uint8_t& h, uint8_t& l);
	void ld_reg_reg(uint8_t& dst, uint8_t v);
	void rl_r(uint8_t& r);
	void rl_rr(uint8_t h, uint8_t l);
	void rr_r(uint8_t& r);
	void rr_rr(uint8_t h, uint8_t l);
	void cp_value(uint8_t v);
	void add_a_value(uint8_t value);
	void add_hl_value(uint16_t value);
	void and_a_value(uint8_t v);

	//Instructions
	void NOP();
	//INC r
	void INC_A(); void INC_B();	void INC_C(); void INC_D();
	void INC_E(); void INC_H(); void INC_L();
	//DEC r
	void DEC_A(); void DEC_B(); void DEC_C(); void DEC_D();
	void DEC_E(); void DEC_H(); void DEC_L();
	//INC rr
	void INC_BC(); void INC_DE(); void INC_HL();
	void INC_SP();
	//INC (rr)
	void INC_MEM_HL();
	//DEC rr
	void DEC_BC(); void DEC_DE(); void DEC_HL();
	void DEC_SP();
	//DEC (rr)
	void DEC_MEM_HL();
	//AND r
	void AND_A(); void AND_B(); void AND_C(); void AND_D();
	void AND_E(); void AND_H(); void AND_L(); void AND_d8();
	//AND (rr)
	void AND_HL();
	//OR r
	//XOR r
	void XOR_A(); void XOR_B(); void XOR_C(); void XOR_D();
	void XOR_E(); void XOR_H(); void XOR_L(); void XOR_d8();
	//XOR (rr)
	void XOR_HL();
	//SUB r
	void SUB_A(); void SUB_B(); void SUB_C(); void SUB_D();
	void SUB_E(); void SUB_H(); void SUB_L(); void SUB_d8();
	//SUB (rr)
	void SUB_HL();
	//ADD A,r
	void ADD_A_A(); void ADD_A_B(); void ADD_A_C(); void ADD_A_D();
	void ADD_A_E(); void ADD_A_H(); void ADD_A_L(); void ADD_A_d8();
	//ADD A,(rr)
	void ADD_A_HL();
	//ADD rr, rr
	void ADD_HL_BC(); void ADD_HL_DE();	void ADD_HL_HL();
	void ADD_HL_SP();
	//ADD rr, s8
	void ADD_SP_s8();
	//LD r,r
	void LD_A_A(); void LD_A_B(); void LD_A_C(); void LD_A_D();
	void LD_A_E(); void LD_A_H(); void LD_A_L();
	void LD_B_A(); void LD_B_B(); void LD_B_C(); void LD_B_D();
	void LD_B_E(); void LD_B_H(); void LD_B_L();
	void LD_C_A(); void LD_C_B(); void LD_C_C(); void LD_C_D();
	void LD_C_E(); void LD_C_H(); void LD_C_L();
	void LD_D_A(); void LD_D_B(); void LD_D_C(); void LD_D_D();
	void LD_D_E(); void LD_D_H(); void LD_D_L();
	void LD_E_A(); void LD_E_B(); void LD_E_C(); void LD_E_D();
	void LD_E_E(); void LD_E_H(); void LD_E_L();
	void LD_H_A(); void LD_H_B(); void LD_H_C(); void LD_H_D();
	void LD_H_E(); void LD_H_H(); void LD_H_L();
	void LD_L_A(); void LD_L_B(); void LD_L_C(); void LD_L_D();
	void LD_L_E(); void LD_L_H(); void LD_L_L();
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
	//LD r,(addr)
	void LD_A_a8(); void LD_A_a16(); void LD_A_aC();
	//CP r
	void CP_A(); void CP_B(); void CP_C(); void CP_D();
	void CP_E(); void CP_H(); void CP_L(); void CP_d8();
	void CP_HL();
	//POP
	void POP_AF(); void POP_BC(); void POP_DE(); void POP_HL();
	//PUSH
	void PUSH_AF(); void PUSH_BC(); void PUSH_DE(); void PUSH_HL();
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
	//RES v,r
	void RES_0_A(); void RES_0_B(); void RES_0_C(); void RES_0_D();
	void RES_0_E(); void RES_0_H(); void RES_0_L(); void RES_0_HL();
	void RES_1_A(); void RES_1_B(); void RES_1_C(); void RES_1_D();
	void RES_1_E(); void RES_1_H(); void RES_1_L(); void RES_1_HL();
	void RES_2_A(); void RES_2_B(); void RES_2_C(); void RES_2_D();
	void RES_2_E(); void RES_2_H(); void RES_2_L(); void RES_2_HL();
	void RES_3_A(); void RES_3_B(); void RES_3_C(); void RES_3_D();
	void RES_3_E(); void RES_3_H(); void RES_3_L(); void RES_3_HL();
	void RES_4_A(); void RES_4_B(); void RES_4_C(); void RES_4_D();
	void RES_4_E(); void RES_4_H(); void RES_4_L(); void RES_4_HL();
	void RES_5_A(); void RES_5_B(); void RES_5_C(); void RES_5_D();
	void RES_5_E(); void RES_5_H(); void RES_5_L(); void RES_5_HL();
	void RES_6_A(); void RES_6_B(); void RES_6_C(); void RES_6_D();
	void RES_6_E(); void RES_6_H(); void RES_6_L(); void RES_6_HL();
	void RES_7_A(); void RES_7_B(); void RES_7_C(); void RES_7_D();
	void RES_7_E(); void RES_7_H(); void RES_7_L(); void RES_7_HL();
	//SET v,r
	//RL r
	void RL_A(); void RL_B(); void RL_C(); void RL_D();
	void RL_E(); void RL_H(); void RL_L(); void RL_HL();
	//RR r
	void RR_A(); void RR_B(); void RR_C(); void RR_D();
	void RR_E(); void RR_H(); void RR_L(); void RR_HL();
	//RLCA, RLA, RRCA, RRA
	void RLCA(); void RLA(); void RRCA(); void RRA();
};