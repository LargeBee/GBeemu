#include "CPU.h"
#include "Instruction.h"

void CPU::buildInstructionTable()
{
	instructions[0x00] = { "NOP", &CPU::NOP, 1, 4 };
}