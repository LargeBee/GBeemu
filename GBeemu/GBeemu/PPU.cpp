#include "PPU.h"
#include "Bus.h"

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

	modeClock = 0;
	mode = MODE_OAM;

	for (int i = 0; i < 0x2000; i++)
	{
		vram[i] = 0;
	}

	for (int i = 0; i < 0xA0; i++)
	{
		oam[i] = 0;
	}

	for (int i = 0; i < 160 * 144; i++)
	{
		framebuffer[i] = 0;
	}
}

void PPU::setBus(Bus& bus)
{
	this->bus = &bus;
}

void PPU::tick()
{
	modeClock++;

	switch (mode)
	{
	case MODE_OAM:
		if (modeClock >= OAM_CYCLES)
		{
			modeClock = 0;
			mode = MODE_TRANSFER;
		}
		break;

	case MODE_TRANSFER:
		if (modeClock >= TRANSFER_CYCLES)
		{
			modeClock = 0;

			renderScanline();

			mode = MODE_HBLANK;
		}
		break;

	case MODE_HBLANK:
		if (modeClock >= HBLANK_CYCLES)
		{
			modeClock = 0;

			ly++;

			if (ly >= 144)
			{
				mode = MODE_VBLANK;
				bus->requestInterrupt(0);
			}
			else
			{
				mode = MODE_OAM;
			}
		}
		break;

	case MODE_VBLANK:
		if (modeClock >= SCANLINE_CYCLES)
		{
			modeClock = 0;

			ly++;

			if (ly > 153)
			{
				ly = 0;
				mode = MODE_OAM;
			}
		}
		break;
	}
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

void PPU::renderScanline()
{
	if (!(lcdc & 0x80))
		return;

	if (!(lcdc & 0x01))
		return;

	uint8_t worldY =
		(ly + scy) & 0xFF;

	uint8_t tileY =
		worldY / 8;

	uint8_t pixelY =
		worldY & 7;

	uint16_t tileMapBase =
		(lcdc & 0x08)
		? 0x1C00
		: 0x1800;

	for (int screenX = 0; screenX < 160; screenX++)
	{
		uint8_t worldX =
			(screenX + scx) & 0xFF;

		uint8_t tileX =
			worldX / 8;

		uint8_t pixelX =
			worldX & 7;

		uint16_t tileMapAddress =
			tileMapBase +
			(tileY * 32) +
			tileX;

		uint8_t tile =
			vram[tileMapAddress];

		uint16_t tileAddress =
			getTileAddress(tile);

		uint8_t bit =
			7 - pixelX;

		uint8_t low =
			(vram[tileAddress + pixelY * 2] >> bit) & 1;

		uint8_t high =
			(vram[tileAddress + pixelY * 2 + 1] >> bit) & 1;

		uint8_t colour =
			(high << 1) | low;

		framebuffer[
			ly * 160 + screenX
		] = colour;
	}
}

uint16_t PPU::getTileAddress(uint8_t tile)
{
	if (lcdc & 0x10)
	{
		return tile * 16;
	}

	int8_t signedTile =
		static_cast<int8_t>(tile);

	return 0x1000 +
		((signedTile + 128) * 16);
}

const uint8_t* PPU::getFramebuffer() const
{
	return framebuffer;
}