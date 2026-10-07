#pragma once

#include <cstdint>

class Bus;

class Timer
{
public:
	Timer();

	void setBus(Bus& bus);

	void reset();
	void tick();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	void setPostBootState();

private:
	Bus* bus;

	uint16_t dividerCounter;

	uint8_t tima;
	uint8_t tma;
	uint8_t tac;

	bool timerEnabled() const;
	uint16_t timerPeriod() const;
	uint8_t timerBit() const;

	void incrementTIMA();
};