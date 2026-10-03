#pragma once
#include <cstdint>

class Emulator;
class Bus;

class CPU
{
public:
	CPU(Emulator& emulator, Bus& bus);

	void tick();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
	
private:
	Emulator& emulator;
	Bus& bus;
};