#include "PPU.h"
#include "Bus.h"
#include <algorithm>

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

	frameReady = false;

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
				frameReady = true;
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

void PPU::renderBackgroundAndWindow()
{
	for (int screenX = 0; screenX < 160; screenX++)
	{
		uint8_t colour = 0;

		// Background
		if (lcdc & 0x01)
		{
			uint8_t worldY =
				(ly + scy) & 0xFF;

			uint8_t worldX =
				(screenX + scx) & 0xFF;

			uint8_t tileY =
				worldY / 8;

			uint8_t tileX =
				worldX / 8;

			uint8_t pixelY =
				worldY & 7;

			uint8_t pixelX =
				worldX & 7;

			uint16_t tileMapBase =
				(lcdc & 0x08)
				? 0x1C00
				: 0x1800;

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

			colour =
				(high << 1) | low;
		}

		// Window
		int windowX =
			static_cast<int>(wx) - 7;

		if ((lcdc & 0x20) &&
			ly >= wy &&
			screenX >= windowX)
		{
			uint8_t windowPixelX =
				screenX - windowX;

			uint8_t windowPixelY =
				ly - wy;

			uint8_t tileX =
				windowPixelX / 8;

			uint8_t tileY =
				windowPixelY / 8;

			uint8_t pixelX =
				windowPixelX & 7;

			uint8_t pixelY =
				windowPixelY & 7;

			uint16_t tileMapBase =
				(lcdc & 0x40)
				? 0x1C00
				: 0x1800;

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

			colour =
				(high << 1) | low;
		}

		bgColourLine[screenX] = colour;

		uint8_t shade =
			(bgp >> (colour * 2)) & 0x03;

		framebuffer[
			ly * 160 + screenX
		] = shade;
	}
}

void PPU::renderSprites()
{
	struct ScanlineSprite
	{
		int index;

		uint8_t x;
		uint8_t y;
		uint8_t tile;
		uint8_t attributes;
	};

	ScanlineSprite sprites[10];

	int spriteCount = 0;

	int spriteHeight =
		(lcdc & 0x04) ? 16 : 8;

	// Find the first 10 sprites in OAM that overlap
	// the current scanline.
	for (int i = 0; i < 40; i++)
	{
		uint8_t y =
			oam[i * 4];

		uint8_t x =
			oam[i * 4 + 1];

		uint8_t tile =
			oam[i * 4 + 2];

		uint8_t attributes =
			oam[i * 4 + 3];

		int spriteY =
			static_cast<int>(y) - 16;

		if (ly < spriteY ||
			ly >= spriteY + spriteHeight)
		{
			continue;
		}

		sprites[spriteCount] =
		{
			i,
			x,
			y,
			tile,
			attributes
		};

		spriteCount++;

		if (spriteCount == 10)
			break;
	}

	// On DMG, lower X has higher priority.
	// If X is the same, lower OAM index has higher priority.
	std::sort(
		sprites,
		sprites + spriteCount,
		[](const ScanlineSprite& a, const ScanlineSprite& b)
		{
			if (a.x != b.x)
				return a.x < b.x;

			return a.index < b.index;
		}
	);

	// Draw lowest priority first so that higher priority
	// sprites overwrite them.
	for (int i = spriteCount - 1; i >= 0; i--)
	{
		ScanlineSprite& sprite =
			sprites[i];

		int spriteY =
			static_cast<int>(sprite.y) - 16;

		int spriteX =
			static_cast<int>(sprite.x) - 8;

		uint8_t tile =
			sprite.tile;

		uint8_t attributes =
			sprite.attributes;

		int pixelY =
			ly - spriteY;

		// Y flip
		if (attributes & 0x40)
		{
			pixelY =
				spriteHeight - 1 - pixelY;
		}

		// In 8x16 mode, bit 0 of the tile number is ignored.
		if (spriteHeight == 16)
		{
			tile &= 0xFE;

			if (pixelY >= 8)
			{
				tile++;
				pixelY -= 8;
			}
		}

		uint16_t tileAddress =
			static_cast<uint16_t>(tile) * 16;

		uint8_t low =
			vram[tileAddress + pixelY * 2];

		uint8_t high =
			vram[tileAddress + pixelY * 2 + 1];

		for (int pixelX = 0; pixelX < 8; pixelX++)
		{
			int screenX =
				spriteX + pixelX;

			if (screenX < 0 || screenX >= 160)
				continue;

			int bit;

			// X flip
			if (attributes & 0x20)
			{
				bit = pixelX;
			}
			else
			{
				bit = 7 - pixelX;
			}

			uint8_t colour =
				(((high >> bit) & 1) << 1) |
				((low >> bit) & 1);

			// Sprite colour 0 is transparent.
			if (colour == 0)
				continue;

			// Attribute bit 7:
			// sprite is behind non-zero BG/window pixels.
			if ((attributes & 0x80) &&
				bgColourLine[screenX] != 0)
			{
				continue;
			}

			// Attribute bit 4 selects the sprite palette.
			uint8_t palette =
				(attributes & 0x10)
				? obp1
				: obp0;

			uint8_t shade =
				(palette >> (colour * 2)) & 0x03;

			framebuffer[
				ly * 160 + screenX
			] = shade;
		}
	}
}

void PPU::renderScanline()
{
	if (!(lcdc & 0x80))
		return;

	renderBackgroundAndWindow();

	if (lcdc & 0x02)
	{
		renderSprites();
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

	return static_cast<uint16_t>(
		0x1000 + signedTile * 16);
}

const uint8_t* PPU::getFramebuffer() const
{
	return framebuffer;
}

bool PPU::isFrameReady() const
{
	return frameReady;
}

void PPU::clearFrameReady()
{
	frameReady = false;
}

uint8_t PPU::getLY() const
{
	return ly;
}

void PPU::setPostBootState()
{
	lcdc = 0x91;
	stat = 0x85;

	scy = 0x00;
	scx = 0x00;

	ly = 0x00;
	lyc = 0x00;

	bgp = 0xFC;
	obp0 = 0xFF;
	obp1 = 0xFF;

	wy = 0x00;
	wx = 0x00;

	modeClock = 0;
	mode = MODE_OAM;

	frameReady = false;
}