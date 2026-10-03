#pragma once
#include <cstdint>

#include "Instruction.h"

class Emulator;
class Bus;


class CPU
{
public:
	CPU(Emulator& emulator, Bus& bus);

	void reset();

	//Execution
	void step();
	void tick();
	void idle();

	//Memory Access
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	//Instruction Fetching
	uint8_t fetch8();
	uint16_t fetch16();
	
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

	//Current instruction
	uint8_t opcode;

	void executeOpcode(uint8_t opcode);
	void executeCB(uint8_t opcode);

	//Instruction Tables
	Instruction instructions[0x100];
	Instruction cbInstructions[0x100];
	void buildInstructionTables();

	//Instruction Helpers

	//Instructions
	void NOP();
};