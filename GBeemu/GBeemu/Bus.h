#pragma once
#include <cstdint>
#include <string>

class Cartridge;
class PPU;
class Timer;
class DMA;
class APU;
class Input;

class Bus
{
public:
	Bus(Cartridge& cartridge);

	bool loadBootROM(const std::string& path);

	void enableBootROM();
	void disableBootROM();

	void connect(
		PPU& ppu,
		Timer& timer,
		DMA& dma,
		APU& apu,
		Input& input
	);

	void reset();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	void requestInterrupt(uint8_t bit);

private:
	Cartridge& cartridge;

	uint8_t bootROM[0x100];
	bool bootROMEnabled;

	PPU* ppu;
	Timer* timer;
	DMA* dma;
	APU* apu;
	Input* input;

	uint8_t wram[0x2000];
	uint8_t hram[0x7F];

	uint8_t interruptEnable;
	uint8_t interruptFlags;
};