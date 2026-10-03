#pragma once

#include <cstdint>

class Bus;

class Input
{
public:
	Input();

	void setBus(Bus& bus);

	void reset();
	void tick();

	uint8_t read();
	void write(uint8_t value);

private:
	Bus* bus;

	uint8_t joyp;

	bool right;
	bool left;
	bool up;
	bool down;

	bool a;
	bool b;
	bool select;
	bool start;
};