#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Cartridge
{
public:
	Cartridge();

	bool loadROM(const std::string& path);
	void reset();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	bool isLoaded() const;
	uint8_t getCartridgeType() const;
	uint8_t getROMSize() const;
	uint8_t getRAMSize() const;

private:
	std::vector < uint8_t > rom;
	std::vector < uint8_t > ram;

	uint8_t cartridgeType;
	uint8_t romSize;
	uint8_t ramSize;

	uint8_t ramBank;

	uint16_t mbc5RomBank;

	bool ramEnable;

	//MBC1
	uint8_t romBankLow;
	uint8_t romBankHigh;
	bool bankingMode;

	//MBC3
	bool rtcSelected;
	uint8_t rtcRegister;

	void loadHeader();
	void allocateRAM();

	uint8_t readROM(uint16_t address);
	uint8_t readRAM(uint16_t address);

	void writeROM(uint16_t address, uint8_t value);
	void writeRAM(uint16_t address, uint8_t value);

	bool hasMBC1() const;
	bool hasMBC2() const;
	bool hasMBC3() const;
	bool hasMBC5() const;
};