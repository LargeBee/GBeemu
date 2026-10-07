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
	joyp = 0x30;

	right = false;
	left = false;
	up = false;
	down = false;

	a = false;
	b = false;
	select = false;
	start = false;

	previousJoyp = read();
}

void Input::tick()
{
	uint8_t currentJoyp = read();

	uint8_t fallingEdges =
		(previousJoyp & ~currentJoyp) & 0x0F;

	if (fallingEdges != 0)
	{
		requestInterrupt();
	}

	previousJoyp = currentJoyp;
}

uint8_t Input::read()
{
	uint8_t value =
		0xC0 |
		(joyp & 0x30) |
		0x0F;

	// P14 - Direction buttons
	if (!(joyp & 0x10))
	{
		if (right)	value &= ~(1 << 0);
		if (left)	value &= ~(1 << 1);
		if (up)		value &= ~(1 << 2);
		if (down)	value &= ~(1 << 3);
	}

	// P15 - Action buttons
	if (!(joyp & 0x20))
	{
		if (a)		value &= ~(1 << 0);
		if (b)		value &= ~(1 << 1);
		if (select)	value &= ~(1 << 2);
		if (start)	value &= ~(1 << 3);
	}

	return value;
}

void Input::write(uint8_t value)
{
	joyp =
		(joyp & ~0x30) |
		(value & 0x30);
}

void Input::setRight(bool pressed) { right = pressed; }
void Input::setLeft(bool pressed) { left = pressed; }
void Input::setUp(bool pressed) { up = pressed; }
void Input::setDown(bool pressed) { down = pressed; }

void Input::setA(bool pressed) { a = pressed; }
void Input::setB(bool pressed) { b = pressed; }
void Input::setSelect(bool pressed) { select = pressed; }
void Input::setStart(bool pressed) { start = pressed; }

void Input::requestInterrupt()
{
	if (bus == nullptr)
		return;

	uint8_t interruptFlags =
		bus->read(0xFF0F);

	interruptFlags |= (1 << 4);

	bus->write(
		0xFF0F,
		interruptFlags
	);
}

void Input::setPostBootState()
{
	reset();
}