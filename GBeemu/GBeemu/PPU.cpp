#include "PPU.h"

PPU::PPU()
{
	bus = nullptr;

	reset();
}

void PPU::reset()
{
	lcdc = 0;
	stat = 0;
	ly = 0;
	lyc = 0;

	scy = 0;
	scx = 0;

	bgp = 0;
	obp0 = 0;
	obp1 = 0;

	wy = 0;
	wx = 0;

	for (int i = 0; i < 0x2000; i++)
	{
		vram[i] = 0;
	}

	for (int i = 0; i < 0xA0; i++)
	{
		oam[i] = 0;
	}
}

void PPU::setBus(Bus& bus)
{
	this->bus = &bus;
}

void PPU::tick()
{
}

uint8_t PPU::read(uint16_t address)
{
	if (address >= 0x8000 && address <= 0x9FFF)
		return vram[address - 0x8000];

	if (address >= 0xFE00 && address <= 0xFE9F)
		return oam[address - 0xFE00];

	switch (address)
	{
	case 0xFF40:
		return lcdc;

	case 0xFF41:
		return stat;

	case 0xFF42:
		return scy;

	case 0xFF43:
		return scx;

	case 0xFF44:
		return ly;

	case 0xFF45:
		return lyc;

	case 0xFF47:
		return bgp;

	case 0xFF48:
		return obp0;

	case 0xFF49:
		return obp1;

	case 0xFF4A:
		return wy;

	case 0xFF4B:
		return wx;
	}

	return 0xFF;
}

void PPU::write(uint16_t address, uint8_t value)
{
	if (address >= 0x8000 && address <= 0x9FFF)
	{
		vram[address - 0x8000] = value;
		return;
	}

	if (address >= 0xFE00 && address <= 0xFE9F)
	{
		oam[address - 0xFE00] = value;
		return;
	}

	switch (address)
	{
	case 0xFF40:
		lcdc = value;
		break;

	case 0xFF41:
		stat = value;
		break;

	case 0xFF42:
		scy = value;
		break;

	case 0xFF43:
		scx = value;
		break;

	case 0xFF44:
		// LY is normally read-only
		break;

	case 0xFF45:
		lyc = value;
		break;

	case 0xFF47:
		bgp = value;
		break;

	case 0xFF48:
		obp0 = value;
		break;

	case 0xFF49:
		obp1 = value;
		break;

	case 0xFF4A:
		wy = value;
		break;

	case 0xFF4B:
		wx = value;
		break;
	}
}

