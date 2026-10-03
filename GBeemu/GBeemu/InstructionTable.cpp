#include "CPU.h"
#include "Instruction.h"

void CPU::buildInstructionTables()
{
	for (int i = 0; i < 0x100; i++)
	{
		instructions[i] = { "UNIMPL", nullptr, 0, 0 };
		cbInstructions[i] = { "CB-UNIMPL", nullptr, 0, 0 };
	}

	//instructions[0x00] = { "NOP", &CPU::NOP, 1, 4 };
}