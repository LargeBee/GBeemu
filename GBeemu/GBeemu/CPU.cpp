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

void CPU::tick(uint8_t cycles)
{
	for (uint8_t i = 0; i < cycles; i++)
	{
		emulator.tick();
	}
}

void CPU::tickM(uint8_t cycles)
{
	for (uint8_t i = 0; i < cycles * 4; i++)
	{
		emulator.tick();
	}
}

void CPU::createTrace(
	uint16_t pc,
	uint8_t opcode,
	const Instruction& instruction
)
{
	lastTrace.pc = pc;
	lastTrace.opcode = opcode;

	lastTrace.a = A;
	lastTrace.f = F;

	lastTrace.b = B;
	lastTrace.c = C;

	lastTrace.d = D;
	lastTrace.e = E;

	lastTrace.h = H;
	lastTrace.l = L;

	lastTrace.sp = SP;

	lastTrace.instruction =
		instruction.mnemonic;
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

	uint16_t currentPC = PC;

	opcode = fetch8();

	tickM(1);

	if (opcode == 0xCB)
	{
		uint8_t cbOpcode = fetch8();

		tickM(1);

		Instruction& instruction =
			cbInstructions[cbOpcode];

		createTrace(
			currentPC,
			cbOpcode,
			instruction
		);

		if (instruction.mnemonic == "CB-UNIMPL")
		{
			emulator.pause();
			return;
		}

		executeCB(cbOpcode);
		return;
	}

	Instruction& instruction =
		instructions[opcode];

	createTrace(
		currentPC,
		opcode,
		instruction
	);

	if (instruction.mnemonic == "UNIMPL")
	{
		emulator.pause();
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

void CPU::write(uint16_t address, uint8_t value)
{
	bus.write(address, value);
}

uint8_t CPU::read(uint16_t address)
{
	return bus.read(address);
}

const TraceEntry& CPU::getLastTrace() const
{
	return lastTrace;
}

bool CPU::getFlag(Flag flag) const
{
	return (F & (1 << flag)) != 0;
}

void CPU::setFlag(Flag flag)
{
	F |= (1 << flag);
}

void CPU::clearFlag(Flag flag)
{
	F &= ~(1 << flag);
}

void CPU::setFlag(Flag flag, bool value)
{
	if (value)
	{
		setFlag(flag);
	}
	else
	{
		clearFlag(flag);
	}
}
