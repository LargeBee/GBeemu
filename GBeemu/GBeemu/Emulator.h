#pragma once
#include <cstdint>

class Bus;
class CPU;
class PPU;
class Timer;
class DMA;
class APU;
class Cartridge;

class Emulator
{
public:
	Emulator();
	~Emulator();

	bool loadROM(const std::string& path);

	void reset();

	//Master clock
	void tick();

	void run();
	void pause();

	void stepCycle();
	void stepInstruction();
	void stepFrame();

	void runCycles(uint64_t cycles);

	bool isRunning() const;

	//Component Access
	CPU& getCPU();
	PPU& getPPU();
	Timer& getTimer();
	DMA& getDMA();
	APU& getAPU();
	Bus& getBus();
	Cartridge& getCartridge();

	uint64_t getCycles() const;

private:
	Cartridge* cartridge;
	Bus* bus;
	CPU* cpu;
	PPU* ppu;
	Timer* timer;
	DMA* dma;
	APU* apu;

	uint64_t cycles;
	bool running;
};