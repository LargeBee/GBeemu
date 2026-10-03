#include "Cartridge.h"

#include <fstream>

Cartridge::Cartridge()
{
	reset();
}

void Cartridge::reset()
{
	rom.clear();
	ram.clear();

	cartridgeType = 0;
	romSize = 0;
	ramSize = 0;

	romBank = 1;
	ramBank = 0;

	ramEnable = false;

	romBankLow = 1;
	romBankHigh = 0;
	bankingMode = false;

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

	if (address < 0x4000)
	{
		return rom[address];
	}

	uint32_t bank = romBank;

	uint32_t offset =
		(bank * 0x4000) +
		(address - 0x4000);

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

	uint32_t offset =
		(ramBank * 0x2000) +
		(address - 0xA000);

	if (offset >= ram.size())
		return 0xFF;

	return ram[offset];
}

void Cartridge::writeROM(uint16_t address, uint8_t value)
{
	if (hasMBC1())
	{
		// MBC1 implementation
		return;
	}

	if (hasMBC2())
	{
		// MBC2 implementation
		return;
	}

	if (hasMBC3())
	{
		// MBC3 implementation
		return;
	}

	if (hasMBC5())
	{
		// MBC5 implementation
		return;
	}
}

void Cartridge::writeRAM(uint16_t address, uint8_t value)
{
	if (!ramEnable)
		return;

	if (ram.empty())
		return;

	uint32_t offset =
		(ramBank * 0x2000) +
		(address - 0xA000);

	if (offset >= ram.size())
		return;

	ram[offset] = value;
}

bool Cartridge::isLoaded() const
{
	return !rom.empty();
}

