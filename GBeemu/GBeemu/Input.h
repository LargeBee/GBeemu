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

	void setRight(bool pressed);
	void setLeft(bool pressed);
	void setUp(bool pressed);
	void setDown(bool pressed);

	void setA(bool pressed);
	void setB(bool pressed);
	void setSelect(bool pressed);
	void setStart(bool pressed);

	void setPostBootState();

private:
	Bus* bus;

	uint8_t joyp;
	uint8_t previousJoyp;

	bool right;
	bool left;
	bool up;
	bool down;

	bool a;
	bool b;
	bool select;
	bool start;

	void requestInterrupt();
};