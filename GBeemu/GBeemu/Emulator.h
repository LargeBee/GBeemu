#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "TraceEntry.h"

class Bus;
class CPU;
class PPU;
class Timer;
class DMA;
class APU;
class Cartridge;
class Input;

class Emulator
{
public:
	Emulator();
	~Emulator();

	bool loadROM(const std::string& path);

	void reset();

	void skipBoot();
	bool runBoot();

	//Master clock
	void tick();

	void run();
	void pause();
	void update();

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

	const uint8_t* getFramebuffer() const;

	const std::vector<TraceEntry>& getTrace() const;

	void clearTrace();

	bool isFrameReady() const;
	void clearFrameReady();

	uint64_t getInstructionCount() const;

	Input& getInput();

private:
	Cartridge* cartridge;
	Bus* bus;
	CPU* cpu;
	PPU* ppu;
	Timer* timer;
	DMA* dma;
	APU* apu;
	Input* input;

	uint64_t cycles;
	bool running;

	uint8_t previousLy = 0;

	uint64_t lastTracedInstruction = 0;

	std::vector<TraceEntry> trace;
};