#pragma once

#include <cstdint>

class Bus;

static constexpr uint16_t OAM_CYCLES = 80;
static constexpr uint16_t TRANSFER_CYCLES = 172;
static constexpr uint16_t HBLANK_CYCLES = 204;
static constexpr uint16_t SCANLINE_CYCLES = 456;

static constexpr uint8_t MODE_HBLANK = 0;
static constexpr uint8_t MODE_VBLANK = 1;
static constexpr uint8_t MODE_OAM = 2;
static constexpr uint8_t MODE_TRANSFER = 3;

class PPU
{
public:
	PPU();

	void setBus(Bus& bus);

	void reset();
	void tick();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	const uint8_t* getFramebuffer() const;

private:
	static constexpr int WIDTH = 160;
	static constexpr int HEIGHT = 144;

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

	uint16_t modeClock;
	uint8_t mode;

	uint8_t framebuffer[WIDTH * HEIGHT];

	void renderScanline();
	uint16_t getTileAddress(uint8_t tile);
};