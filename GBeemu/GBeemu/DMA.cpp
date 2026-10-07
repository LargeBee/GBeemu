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
	cycle = 0;
	active = false;
}

void DMA::tick()
{
	if (!active || bus == nullptr)
		return;

	cycle++;

	if (cycle < 4)
		return;

	cycle = 0;

	uint16_t sourceAddress =
		(static_cast<uint16_t>(source) << 8) |
		index;

	uint16_t destinationAddress =
		0xFE00 + index;

	uint8_t value =
		bus->read(sourceAddress);

	bus->write(destinationAddress, value);

	index++;

	if (index >= 0xA0)
	{
		for (uint16_t i = 0; i < 0xA0; i++)
		{
			uint8_t sourceValue = bus->read(0xC300 + i);
			uint8_t oamValue = bus->read(0xFE00 + i);

			if (sourceValue != oamValue)
			{
				// Put breakpoint here
				int debug = 0;
			}
		}
		active = false;
		index = 0;
	}
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
		cycle = 0;
		active = true;
	}
}