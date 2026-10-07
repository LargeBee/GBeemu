#include "CPU.h"
#include "Emulator.h"
#include "Bus.h"

CPU::CPU(Emulator& emulator, Bus& bus)
	: emulator(emulator),
	bus(bus)
{
	traceFile.open("trace.txt");
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
	imeEnableDelay = 0;
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

	lastTrace.scanline = read(0xFF44);
	
	
	//std::ofstream traceFile("trace.txt", std::ios::app);

	/*traceFile
		<< "A: " << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int)lastTrace.a
		<< " F: " << std::setw(2) << (int)lastTrace.f
		<< " B: " << std::setw(2) << (int)lastTrace.b
		<< " C: " << std::setw(2) << (int)lastTrace.c
		<< " D: " << std::setw(2) << (int)lastTrace.d
		<< " E: " << std::setw(2) << (int)lastTrace.e
		<< " H: " << std::setw(2) << (int)lastTrace.h
		<< " L: " << std::setw(2) << (int)lastTrace.l
		<< " SP: " << std::setw(4) << (int)lastTrace.sp
		<< " PC: 00:" << std::setw(4) << (int)lastTrace.pc
		<< " (";

	// Output the opcode and following 3 bytes
	for (int i = 0; i < 4; i++)
	{
		uint8_t value = read(pc + i);

		traceFile << std::setw(2) << (int)value;

		if (i < 3)
			traceFile << " ";
	}

	traceFile << ")\n";*/
	
}

void CPU::idle()
{
	emulator.tick();
}

void CPU::step()
{
	if (stopped)
	{
		return;
	}

	if (handleInterrupts())
		return;

	if (halted)
	{
		idle();
		return;
	}

	uint16_t currentPC = PC;
	if (haltBug)
	{
		opcode = read(PC);
		haltBug = false;
	}
	else
	{
		opcode = fetch8();
	}

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

		updateIME();

		instructionCount++;
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

	updateIME();

	instructionCount++;
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

uint64_t CPU::getInstructionCount() const
{
	return instructionCount;
}

void CPU::updateIME()
{
	if (imeEnableDelay > 0)
	{
		imeEnableDelay--;

		if (imeEnableDelay == 0)
		{
			ime = true;
		}
	}
}

bool CPU::handleInterrupts()
{
	uint8_t IE = read(0xFFFF);
	uint8_t IF = read(0xFF0F);

	uint8_t pending =
		IE & IF & 0x1F;

	if (pending == 0)
		return false;

	if (halted)
	{
		halted = false;
	}

	if (!ime)
		return false;

	ime = false;

	uint8_t interruptBit;
	uint16_t vector;

	if (pending & 0x01)
	{
		interruptBit = 0;
		vector = 0x0040;
	}
	else if (pending & 0x02)
	{
		interruptBit = 1;
		vector = 0x0048;
	}
	else if (pending & 0x04)
	{
		interruptBit = 2;
		vector = 0x0050;
	}
	else if (pending & 0x08)
	{
		interruptBit = 3;
		vector = 0x0058;
	}
	else
	{
		interruptBit = 4;
		vector = 0x0060;
	}

	tickM(2);

	IF = read(0xFF0F);
	IF &= ~(1 << interruptBit);
	write(0xFF0F, IF);

	SP--;
	write(SP, PC >> 8);
	tickM(1);

	SP--;
	write(SP, PC & 0xFF);
	tickM(1);

	PC = vector;

	tickM(1);

	return true;
}

void CPU::wakeFromStop()
{
	stopped = false;
}

bool CPU::isStopped() const
{
	return stopped;
}