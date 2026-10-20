#include "Timer.h"
#include "Bus.h"

Timer::Timer()
{
	bus = nullptr;
	reset();
}

void Timer::setBus(Bus& bus)
{
	this->bus = &bus;
}

void Timer::reset()
{
	dividerCounter = 0;

	tima = 0;
	tma = 0;
	tac = 0;
}

void Timer::tick()
{
	uint8_t bit = timerBit();

	bool oldSignal =
		timerEnabled() &&
		((dividerCounter >> bit) & 1);

	bool oldAPUSignal = (dividerCounter & 0x1000) != 0;

	dividerCounter++;

	bool newSignal =
		timerEnabled() &&
		((dividerCounter >> bit) & 1);

	bool newAPUSignal = (dividerCounter & 0x1000) != 0;

	if (oldSignal && !newSignal)
	{
		incrementTIMA();
	}

	if (oldAPUSignal && !newAPUSignal)
	{
		pendingAPUClocks++;
	}
}

uint8_t Timer::timerBit() const
{
	switch (tac & 0x03)
	{
	case 0:
		return 9;

	case 1:
		return 3;

	case 2:
		return 5;

	case 3:
		return 7;
	}

	return 9;
}

uint16_t Timer::timerPeriod() const
{
	switch (tac & 0x03)
	{
	case 0:
		return 1024;

	case 1:
		return 16;

	case 2:
		return 64;

	case 3:
		return 256;
	}

	return 1024;
}

bool Timer::timerEnabled() const
{
	return (tac & 0x04) != 0;
}

void Timer::incrementTIMA()
{
	if (tima == 0xFF)
	{
		tima = tma;

		// Timer interrupt
		bus->requestInterrupt(2);
	}
	else
	{
		tima++;
	}
}

uint8_t Timer::read(uint16_t address)
{
	switch (address)
	{
	case 0xFF04:
		return dividerCounter >> 8;

	case 0xFF05:
		return tima;

	case 0xFF06:
		return tma;

	case 0xFF07:
		return tac | 0xF8;
	}

	return 0xFF;
}

void Timer::write(uint16_t address, uint8_t value)
{
	switch (address)
	{
	case 0xFF04:
		if (dividerCounter & 0x1000)
			pendingAPUClocks++;

		dividerCounter = 0;
		break;

	case 0xFF05:
		tima = value;
		break;

	case 0xFF06:
		tma = value;
		break;

	case 0xFF07:
		tac = value & 0x07;
		break;
	}
}

void Timer::setPostBootState()
{
	tima = 0x00;
	tma = 0x00;
	tac = 0x00;

	dividerCounter = 0xABCC;
}

uint16_t Timer::getDividerCounter() const
{
	return dividerCounter;
}

uint32_t Timer::consumeAPUClocks()
{
	uint32_t count = pendingAPUClocks;
	pendingAPUClocks = 0;
	return count;
}