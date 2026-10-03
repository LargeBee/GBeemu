#pragma once

#include <cstdint>

class CPU;

struct Instruction
{
	const char* mnemonic;

	void (CPU::* execute)();

	uint8_t length;
	uint8_t cycles;
};