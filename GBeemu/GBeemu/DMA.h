#pragma once

#include <cstdint>

class Bus;

class DMA
{
public:
	DMA();

	void setBus(Bus& bus);

	void reset();
	void tick();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

private:
	Bus* bus;

	uint8_t source;
	uint16_t index;

	bool active;
};