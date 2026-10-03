#pragma once

#include <cstdint>

class Bus;

class PPU
{
public:
	PPU();

	void setBus(Bus& bus);

	void reset();
	void tick();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

private:
	Bus* bus;

	uint8_t vram[0x2000];
	uint8_t oam[0xA0];

	uint8_t lcdc;
	uint8_t stat;
	uint8_t ly;
	uint8_t lyc;

	uint8_t scy;
	uint8_t scx;

	uint8_t bgp;
	uint8_t obp0;
	uint8_t obp1;

	uint8_t wy;
	uint8_t wx;
};