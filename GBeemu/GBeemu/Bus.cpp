#include "Bus.h"

#include "Cartridge.h"
//#include "PPU.h"
//#include "Timer.h"
//#include "DMA.h"
//#include "APU.h"
//#include "Input.h"

Bus::Bus(
	Cartridge& cartridge,
	PPU& ppu,
	Timer& timer,
	DMA& dma,
	APU& apu,
	Input& input
)
	: cartridge(cartridge),
	ppu(ppu),
	timer(timer),
	dma(dma),
	apu(apu),
	input(input)
{
	reset();
}

void Bus::reset()
{
	for (int i = 0; i < 0x2000; i++)
		wram[i] = 0;

	for (int i = 0; i < 0x7F; i++)
		hram[i] = 0;

	interruptEnable = 0;
	interruptFlags = 0;
}

uint8_t Bus::read(uint16_t address)
{
	// Cartridge ROM
	if (address <= 0x7FFF)
		return cartridge.read(address);

	// VRAM
	if (address >= 0x8000 && address <= 0x9FFF)
		return ppu.readVRAM(address - 0x8000);

	// Cartridge RAM
	if (address >= 0xA000 && address <= 0xBFFF)
		return cartridge.read(address - 0xA000);

	// Work RAM
	if (address >= 0xC000 && address <= 0xDFFF)
		return wram[address - 0xC000];

	// Echo RAM
	if (address >= 0xE000 && address <= 0xFDFF)
		return wram[address - 0xE000];

	// OAM
	if (address >= 0xFE00 && address <= 0xFE9F)
		return ppu.readOAM(address - 0xFE00);

	// Unusable memory
	if (address >= 0xFEA0 && address <= 0xFEFF)
		return 0xFF;

	// I/O
	if (address >= 0xFF00 && address <= 0xFF7F)
	{
		switch (address)
		{
		case 0xFF00:
			return input.read();

		case 0xFF04:
		case 0xFF05:
		case 0xFF06:
		case 0xFF07:
			return timer.read(address);

		case 0xFF0F:
			return interruptFlags;

		case 0xFF10:
		case 0xFF11:
		case 0xFF12:
		case 0xFF13:
		case 0xFF14:
		case 0xFF15:
		case 0xFF16:
		case 0xFF17:
		case 0xFF18:
		case 0xFF19:
		case 0xFF1A:
		case 0xFF1B:
		case 0xFF1C:
		case 0xFF1D:
		case 0xFF1E:
		case 0xFF20:
		case 0xFF21:
		case 0xFF22:
		case 0xFF23:
		case 0xFF24:
		case 0xFF25:
		case 0xFF26:
			return apu.read(address);

		case 0xFF40:
		case 0xFF41:
		case 0xFF42:
		case 0xFF43:
		case 0xFF44:
		case 0xFF45:
		case 0xFF47:
		case 0xFF48:
		case 0xFF49:
		case 0xFF4A:
		case 0xFF4B:
			return ppu.readRegister(address);

		default:
			return 0xFF;
		}
	}

	// HRAM
	if (address >= 0xFF80 && address <= 0xFFFE)
		return hram[address - 0xFF80];

	// Interrupt Enable
	if (address == 0xFFFF)
		return interruptEnable;

	return 0xFF;
}

void Bus::write(uint16_t address, uint8_t value)
{
	// Cartridge ROM / MBC registers
	if (address <= 0x7FFF)
	{
		cartridge.write(address, value);
		return;
	}

	// VRAM
	if (address >= 0x8000 && address <= 0x9FFF)
	{
		ppu.writeVRAM(address, value);
		return;
	}

	// Cartridge RAM
	if (address >= 0xA000 && address <= 0xBFFF)
	{
		cartridge.write(address, value);
		return;
	}

	// Work RAM
	if (address >= 0xC000 && address <= 0xDFFF)
	{
		wram[address - 0xC000] = value;
		return;
	}

	// Echo RAM
	if (address >= 0xE000 && address <= 0xFDFF)
	{
		wram[address - 0xE000] = value;
		return;
	}

	// OAM
	if (address >= 0xFE00 && address <= 0xFE9F)
	{
		ppu.writeOAM(address, value);
		return;
	}

	// Unusable
	if (address >= 0xFEA0 && address <= 0xFEFF)
		return;

	// I/O
	if (address >= 0xFF00 && address <= 0xFF7F)
	{
		switch (address)
		{
		case 0xFF00:
			input.write(value);
			return;

		case 0xFF04:
		case 0xFF05:
		case 0xFF06:
		case 0xFF07:
			timer.write(address, value);
			return;

		case 0xFF0F:
			interruptFlags = value;
			return;

		case 0xFF10:
		case 0xFF11:
		case 0xFF12:
		case 0xFF13:
		case 0xFF14:
		case 0xFF15:
		case 0xFF16:
		case 0xFF17:
		case 0xFF18:
		case 0xFF19:
		case 0xFF1A:
		case 0xFF1B:
		case 0xFF1C:
		case 0xFF1D:
		case 0xFF1E:
		case 0xFF20:
		case 0xFF21:
		case 0xFF22:
		case 0xFF23:
		case 0xFF24:
		case 0xFF25:
		case 0xFF26:
			apu.write(address, value);
			return;

		case 0xFF40:
		case 0xFF41:
		case 0xFF42:
		case 0xFF43:
		case 0xFF44:
		case 0xFF45:
		case 0xFF47:
		case 0xFF48:
		case 0xFF49:
		case 0xFF4A:
		case 0xFF4B:
			ppu.writeRegister(address, value);
			return;

		default:
			return;
		}
	}

	// HRAM
	if (address >= 0xFF80 && address <= 0xFFFE)
	{
		hram[address - 0xFF80] = value;
		return;
	}

	// Interrupt Enable
	if (address == 0xFFFF)
	{
		interruptEnable = value;
		return;
	}
}