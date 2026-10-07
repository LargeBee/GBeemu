#include "Emulator.h"

#include "Bus.h"
#include "CPU.h"
#include "PPU.h"
#include "Timer.h"
#include "DMA.h"
#include "APU.h"
#include "Cartridge.h"
#include "Input.h"

#include <windows.h>

Emulator::Emulator()
{
	cycles = 0;
	running = false;
	skipBootFlag = false;

	cartridge = new Cartridge();
	
	ppu = new PPU();
	timer = new Timer();
	dma = new DMA();
	apu = new APU();
	input = new Input();

	bus = new Bus(*cartridge);

	bus->connect(
		*ppu,
		*timer,
		*dma,
		*apu,
		*input
	);

	ppu->setBus(*bus);
	timer->setBus(*bus);
	dma->setBus(*bus);
	apu->setBus(*bus);
	input->setBus(*bus);

	cpu = new CPU(*this, *bus);
}

Emulator::~Emulator()
{
	cartridge->flushSave();

	delete cpu;
	delete apu;
	delete dma;
	delete timer;
	delete ppu;
	delete bus;
	delete cartridge;
}

void Emulator::reset()
{
	cycles = 0;
	running = false;

	cartridge->reset();
	bus->reset();
	ppu->reset();
	timer->reset();
	dma->reset();
	apu->reset();
	cpu->reset();
}

void Emulator::tick()
{
	cycles++;
	ppu->tick();

	/*if (ppu->getLY() != previousLy)
	{
		char buffer[64];

		sprintf_s(
			buffer,
			"LY %02X at %llu T\n",
			ppu->getLY(),
			static_cast<unsigned long long>(cycles)
		);

		OutputDebugStringA(buffer);
		previousLy = ppu->getLY();
	}*/

	timer->tick();
	dma->tick();
	//apu->tick();
	input->tick();
}

void Emulator::update()
{
	if (!running)
		return;

	while (!isFrameReady() && running)
	{
		if (cpu->isStopped())
		{
			break;
		}

		stepInstruction();
	}

	if (isFrameReady())
	{
		cartridge->tickSave();
	}
}

void Emulator::stepInstruction()
{
	cpu->step();

	if (cpu->getInstructionCount() == lastTracedInstruction)
		return;

	lastTracedInstruction = cpu->getInstructionCount();

	trace.push_back(
		cpu->getLastTrace()
	);

	if (trace.size() > 100)
	{
		trace.erase(trace.begin());
	}
}

const std::vector<TraceEntry>& Emulator::getTrace() const
{
	return trace;
}

void Emulator::clearTrace()
{
	trace.clear();
}

/*bool Emulator::runBoot()
{
	reset();

	if (!bus->loadBootROM("dmg_boot.bin"))
	{
		return false;
	}

	bus->enableBootROM();

	running = true;

	return true;
}*/

void Emulator::run()
{
	running = true;
}

void Emulator::pause()
{
	running = false;
}

void Emulator::stepCycle()
{
	tick();
}

void Emulator::stepFrame()
{
	uint64_t start = cycles;

	while (cycles - start < 70224)
	{
		cpu->step();
	}
}

void Emulator::runCycles(uint64_t cycleCount)
{
	uint64_t start = cycles;

	while (cycles - start < cycleCount)
	{
		cpu->step();
	}
}

bool Emulator::isRunning() const
{
	return running;
}

CPU& Emulator::getCPU()
{
	return *cpu;
}

PPU& Emulator::getPPU()
{
	return *ppu;
}

Timer& Emulator::getTimer()
{
	return *timer;
}

DMA& Emulator::getDMA()
{
	return *dma;
}

APU& Emulator::getAPU()
{
	return *apu;
}

Bus& Emulator::getBus()
{
	return *bus;
}

Cartridge& Emulator::getCartridge()
{
	return *cartridge;
}

uint64_t Emulator::getCycles() const
{
	return cycles;
}

bool Emulator::loadROM(const std::string& path)
{
	cartridge->flushSave();

	reset();

	if (!cartridge->loadROM(path))
		return false;

	if (skipBootFlag)
	{
		skipBoot();
	}
	else
	{
		if (!bus->loadBootROM("dmg_boot.bin"))
		{
			return false;
		}
		bus->enableBootROM();
	}
	
	running = true;

	return true;
}

const uint8_t* Emulator::getFramebuffer() const
{
	return ppu->getFramebuffer();
}

bool Emulator::isFrameReady() const
{
	return ppu->isFrameReady();
}

void Emulator::clearFrameReady()
{
	ppu->clearFrameReady();
}

uint64_t Emulator::getInstructionCount() const
{
	return cpu->getInstructionCount();
}

Input& Emulator::getInput()
{
	return *input;
}

void Emulator::skipBoot()
{
	cpu->setPostBootState();
	ppu->setPostBootState();
	timer->setPostBootState();
	input->setPostBootState();
	bus->setPostBootState();

	bus->disableBootROM();
}