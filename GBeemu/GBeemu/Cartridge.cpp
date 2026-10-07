#include "Cartridge.h"

#include <fstream>

Cartridge::Cartridge()
{
	reset();
}

void Cartridge::reset()
{
	ramBank = 0;

	ramEnable = false;

	romBankLow = 1;
	romBankHigh = 0;
	bankingMode = false;

	mbc5RomBank = 1;

	rtcSelected = false;
	rtcRegister = 0;
}

bool Cartridge::loadROM(const std::string& path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (!file)
		return false;

	std::streamsize size = file.tellg();

	if (size <= 0)
		return false;

	file.seekg(0, std::ios::beg);

	rom.resize(static_cast<size_t>(size));

	if (!file.read(
		reinterpret_cast<char*>(rom.data()),
		size
	))
	{
		rom.clear();
		return false;
	}

	loadHeader();
	allocateRAM();

	return true;
}

void Cartridge::loadHeader()
{
	cartridgeType = rom[0x0147];
	romSize = rom[0x0148];
	ramSize = rom[0x0149];
}

void Cartridge::allocateRAM()
{
	if (hasMBC2())
	{
		ram.resize(0x0200);
		return;
	}

	switch (ramSize)
	{
	case 0x00:
		ram.resize(0);
		break;

	case 0x01:
		ram.resize(0x0800);
		break;

	case 0x02:
		ram.resize(0x2000);
		break;

	case 0x03:
		ram.resize(0x8000);
		break;

	case 0x04:
		ram.resize(0x20000);
		break;

	case 0x05:
		ram.resize(0x10000);
		break;

	default:
		ram.resize(0);
		break;
	}
}

bool Cartridge::hasMBC1() const
{
	return cartridgeType == 0x01 ||
		cartridgeType == 0x02 ||
		cartridgeType == 0x03;
}

bool Cartridge::hasMBC2() const
{
	return cartridgeType == 0x05 ||
		cartridgeType == 0x06;
}

bool Cartridge::hasMBC3() const
{
	return cartridgeType >= 0x0F &&
		cartridgeType <= 0x13;
}

bool Cartridge::hasMBC5() const
{
	return cartridgeType >= 0x19 &&
		cartridgeType <= 0x1E;
}

uint8_t Cartridge::read(uint16_t address)
{
	if (address <= 0x7FFF)
		return readROM(address);

	if (address >= 0xA000 && address <= 0xBFFF)
		return readRAM(address);

	return 0xFF;
}

void Cartridge::write(uint16_t address, uint8_t value)
{
	if (address <= 0x7FFF)
	{
		writeROM(address, value);
		return;
	}

	if (address >= 0xA000 && address <= 0xBFFF)
	{
		writeRAM(address, value);
		return;
	}
}

uint8_t Cartridge::readROM(uint16_t address)
{
	if (rom.empty())
		return 0xFF;

	uint32_t bank = 0;

	if (address < 0x4000)
	{
		if (hasMBC1() && bankingMode)
		{
			bank =
				static_cast<uint32_t>(romBankHigh) << 5;
		}
	}
	else
	{
		if (hasMBC1())
		{
			bank =
				(static_cast<uint32_t>(romBankHigh) << 5) |
				romBankLow;
		}
		else if (hasMBC2())
		{
			bank = romBankLow;
		}
		else if (hasMBC3())
		{
			bank = romBankLow;
		}
		else if (hasMBC5())
		{
			bank = mbc5RomBank;
		}
		else
		{
			bank = 1;
		}
	}

	uint32_t offset;

	if (address < 0x4000)
	{
		offset =
			(bank * 0x4000) +
			address;
	}
	else
	{
		offset =
			(bank * 0x4000) +
			(address - 0x4000);
	}

	if (offset >= rom.size())
		return 0xFF;

	return rom[offset];
}

uint8_t Cartridge::readRAM(uint16_t address)
{
	if (!ramEnable)
		return 0xFF;

	if (ram.empty())
		return 0xFF;

	if (hasMBC2())
	{
		uint16_t offset =
			(address - 0xA000) & 0x01FF;

		return 0xF0 | ram[offset];
	}

	uint32_t offset =
		(static_cast<uint32_t>(ramBank) * 0x2000) +
		(address - 0xA000);

	if (offset >= ram.size())
		return 0xFF;

	return ram[offset];
}

void Cartridge::writeROM(uint16_t address, uint8_t value)
{
	if (hasMBC1())
	{
		if (address <= 0x1FFF)
		{
			ramEnable =
				(value & 0x0F) == 0x0A;
		}
		else if (address <= 0x3FFF)
		{
			romBankLow =
				value & 0x1F;

			if (romBankLow == 0)
			{
				romBankLow = 1;
			}
		}
		else if (address <= 0x5FFF)
		{
			romBankHigh =
				value & 0x03;

			if (bankingMode)
			{
				ramBank = romBankHigh;
			}
			else
			{
				ramBank = 0;
			}
		}
		else if (address <= 0x7FFF)
		{
			bankingMode =
				(value & 0x01) != 0;

			if (bankingMode)
			{
				ramBank = romBankHigh;
			}
			else
			{
				ramBank = 0;
			}
		}

		return;
	}

	if (hasMBC2())
	{
		if (address <= 0x3FFF)
		{
			if ((address & 0x0100) == 0)
			{
				ramEnable =
					(value & 0x0F) == 0x0A;
			}
			else
			{
				romBankLow =
					value & 0x0F;

				if (romBankLow == 0)
				{
					romBankLow = 1;
				}
			}
		}

		return;
	}

	if (hasMBC3())
	{
		if (address <= 0x1FFF)
		{
			ramEnable =
				(value & 0x0F) == 0x0A;
		}
		else if (address <= 0x3FFF)
		{
			romBankLow =
				value & 0x7F;

			if (romBankLow == 0)
			{
				romBankLow = 1;
			}
		}
		else if (address <= 0x5FFF)
		{
			if (value <= 0x03)
			{
				ramBank = value;
				rtcSelected = false;
			}
			else if (value >= 0x08 && value <= 0x0C)
			{
				rtcRegister = value;
				rtcSelected = true;
			}
		}
		else if (address <= 0x7FFF)
		{
			// RTC latch
		}

		return;
	}

	if (hasMBC5())
	{
		if (address <= 0x1FFF)
		{
			ramEnable =
				(value & 0x0F) == 0x0A;
		}
		else if (address <= 0x2FFF)
		{
			mbc5RomBank =
				(mbc5RomBank & 0x0100) |
				value;
		}
		else if (address <= 0x3FFF)
		{
			mbc5RomBank =
				(mbc5RomBank & 0x00FF) |
				((value & 0x01) << 8);
		}
		else if (address <= 0x5FFF)
		{
			ramBank =
				value & 0x0F;
		}

		return;
	}
}

void Cartridge::writeRAM(uint16_t address, uint8_t value)
{
	if (!ramEnable)
		return;

	if (ram.empty())
		return;

	if (hasMBC2())
	{
		uint16_t offset =
			(address - 0xA000) & 0x01FF;

		ram[offset] =
			value & 0x0F;

		return;
	}

	uint32_t offset =
		(static_cast<uint32_t>(ramBank) * 0x2000) +
		(address - 0xA000);

	if (offset >= ram.size())
		return;

	ram[offset] = value;
}

bool Cartridge::isLoaded() const
{
	return !rom.empty();
}

