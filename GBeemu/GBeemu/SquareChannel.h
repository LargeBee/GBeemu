
#pragma once

#include <cstdint>

class SquareChannel
{
public:
	SquareChannel(uint16_t baseAddress, bool hasSweep = false);

	void reset();
	void tick();
	void clockLength();
	void clockSweep();
	void clockEnvelope();

	uint8_t read(uint16_t address) const;
	void write(uint16_t address, uint8_t value);

	float getSample() const;
	bool isEnabled() const;

private:
	void trigger();

	bool enabled = false;
	bool dacEnabled = false;

	uint8_t nr21 = 0;
	uint8_t nr22 = 0;
	uint8_t nr23 = 0;
	uint8_t nr24 = 0;

	uint8_t dutyPosition = 0;

	uint16_t frequencyTimer = 0;
	uint16_t lengthCounter = 0;

	uint8_t currentVolume = 0;
	uint8_t envelopeTimer = 0;

	static constexpr uint8_t DUTY_TABLE[4][8] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 1, 1, 1 },
		{ 0, 1, 1, 1, 1, 1, 1, 0 }
	};

	bool hasSweep = false;
	uint16_t baseAddress;

	uint8_t nr10 = 0;

	uint16_t shadowFrequency = 0;
	uint8_t sweepTimer = 0;
	bool sweepEnabled = false;
	bool sweepNegateUsed = false;

	uint16_t calculateSweepFrequency();
};
