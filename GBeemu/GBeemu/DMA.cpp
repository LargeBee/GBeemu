#include "DMA.h"
#include "Bus.h"

DMA::DMA()
{
	bus = nullptr;

	reset();
}

void DMA::setBus(Bus& bus)
{
	this->bus = &bus;
}

void DMA::reset()
{
	source = 0;
	index = 0;
	active = false;
}

void DMA::tick()
{
	if (!active)
		return;

	// DMA implementation will go here.
}

uint8_t DMA::read(uint16_t address)
{
	if (address == 0xFF46)
		return source;

	return 0xFF;
}

void DMA::write(uint16_t address, uint8_t value)
{
	if (address == 0xFF46)
	{
		source = value;
		index = 0;
		active = true;
	}
}