#pragma once

#include <cstdint>

class Bus;

class APU
{
public:
	APU();

	void setBus(Bus& bus);

	void reset();
	void tick();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

private:
	Bus* bus;

	uint8_t registers[0x30];
};