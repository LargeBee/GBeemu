
#pragma once

#include <cstdint>

class NoiseChannel
{
public:
	NoiseChannel();

	void reset();
	void tick();

	void clockLength();
	void clockEnvelope();

	uint8_t read(uint16_t address) const;
	void write(uint16_t address, uint8_t value);

	float getSample() const;
	bool isEnabled() const;

private:
	void trigger();

	bool enabled = false;
	bool dacEnabled = false;

	uint8_t nr41 = 0;
	uint8_t nr42 = 0;
	uint8_t nr43 = 0;
	uint8_t nr44 = 0;

	uint16_t lengthCounter = 0;
	uint32_t frequencyTimer = 0;

	uint32_t getTimerPeriod() const;

	uint16_t lfsr = 0x7FFF;

	uint8_t currentVolume = 0;
	uint8_t envelopeTimer = 0;
};
