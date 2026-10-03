#include "CPU.h"
#include "Emulator.h"
#include "Bus.h"

CPU::CPU(Emulator& emulator, Bus& bus)
	: emulator(emulator),
	bus(bus)
{
	reset();

	buildInstructionTables();
}

void CPU::reset()
{
	A = 0;
	F = 0;

	B = 0;
	C = 0;

	D = 0;
	E = 0;

	H = 0;
	L = 0;

	PC = 0;
	SP = 0;

	ime = false;
	halted = false;
	stopped = false;

	opcode = 0;
}

void CPU::tick()
{
	emulator.tick();
}

void CPU::idle()
{
	emulator.tick();
}

void CPU::step()
{
	if (halted)
	{
		idle();
		return;
	}

	opcode = fetch8();

	if (opcode == 0xCB)
	{
		uint8_t cbOpcode = fetch8();

		executeCB(cbOpcode);
		return;
	}

	executeOpcode(opcode);
}

void CPU::executeOpcode(uint8_t opcode)
{
	Instruction& instruction = instructions[opcode];

	if (instruction.execute == nullptr)
	{
		return;
	}

	(this->*instruction.execute)();
}

void CPU::executeCB(uint8_t opcode)
{
	Instruction& instruction = cbInstructions[opcode];

	if (instruction.execute == nullptr)
	{
		// Unimplemented CB opcode
		return;
	}

	(this->*instruction.execute)();
}

uint8_t CPU::fetch8()
{
	uint8_t value = read(PC);
	
	PC++;

	return value;
}

uint16_t CPU::fetch16()
{
	uint8_t low = fetch8();
	uint8_t high = fetch8();

	return low | (high << 8);
}

uint8_t CPU::read(uint16_t address)
{
	uint8_t value = bus.read(address);

	tick();

	return value;
}