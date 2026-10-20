
#pragma once

#include <cstdint>
#include <array>

class WaveChannel
{
public:
	WaveChannel();

	void powerOff();

	void reset();
	void tick();
	void clockLength();

	uint8_t read(uint16_t address) const;
	void write(uint16_t address, uint8_t value);

	float getSample() const;
	bool isEnabled() const;

private:
	void trigger();

	bool enabled = false;
	bool dacEnabled = false;

	uint8_t nr30 = 0;
	uint8_t nr31 = 0;
	uint8_t nr32 = 0;
	uint8_t nr33 = 0;
	uint8_t nr34 = 0;

	std::array<uint8_t, 16> waveRAM{};

	uint16_t frequencyTimer = 0;
	uint16_t lengthCounter = 0;

	uint8_t samplePosition = 0;
	uint8_t sampleBuffer = 0;
};
