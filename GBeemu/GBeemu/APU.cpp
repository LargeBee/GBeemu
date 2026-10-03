#include "APU.h"
#include "Bus.h"

APU::APU()
{
	bus = nullptr;

	reset();
}

void APU::setBus(Bus& bus)
{
	this->bus = &bus;
}

void APU::reset()
{
	for (int i = 0; i < 0x30; i++)
	{
		registers[i] = 0;
	}
}

void APU::tick()
{
}

uint8_t APU::read(uint16_t address)
{
	if (address >= 0xFF10 && address <= 0xFF3F)
		return registers[address - 0xFF10];

	return 0xFF;
}

void APU::write(uint16_t address, uint8_t value)
{
	if (address >= 0xFF10 && address <= 0xFF3F)
	{
		registers[address - 0xFF10] = value;
	}
}