#include "Input.h"
#include "Bus.h"

Input::Input()
{
	bus = nullptr;

	reset();
}

void Input::setBus(Bus& bus)
{
	this->bus = &bus;
}

void Input::reset()
{
	joyp = 0xCF;

	right = false;
	left = false;
	up = false;
	down = false;

	a = false;
	b = false;
	select = false;
	start = false;
}

void Input::tick()
{
}

uint8_t Input::read()
{
	return joyp;
}

void Input::write(uint8_t value)
{
	joyp = (value & 0x30) | 0xC0;
}