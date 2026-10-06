#pragma once

#include <cstdint>
#include <string>

struct TraceEntry
{
	uint16_t pc;
	uint8_t opcode;

	uint8_t a;
	uint8_t f;

	uint8_t b;
	uint8_t c;

	uint8_t d;
	uint8_t e;

	uint8_t h;
	uint8_t l;

	uint16_t sp;

	std::string instruction;
	bool implemented;

	//test
	uint8_t scanline;
};