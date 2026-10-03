#include "Emulator.h"

#include "Bus.h"
#include "CPU.h"
//#include "PPU.h"
//#include "timer.h"
//#include "DMA.h"
//#include "APU.h"
//#include "Cartridge.h"

Emulator::Emulator()
{
	cycles = 0;
	running = false;

	cartridge = new Cartridge();
	bus = new Bus(*cartridge);
	ppu = new PPU(*bus);
	timer = new Timer(*bus);
	dma = new DMA(*bus);
	apu = new APU(*bus);
	cpu = new CPU(*this, *bus);
}

Emulator::~Emulator()
{
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
	ppu->tick();
	timer->tick();
	dma->tick();
	apu->tick();

	cycles++;
}

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

void Emulator::stepInstruction()
{
	cpu->step();
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

