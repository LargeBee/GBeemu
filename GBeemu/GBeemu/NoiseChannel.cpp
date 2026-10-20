
#include "NoiseChannel.h"

NoiseChannel::NoiseChannel()
{
	reset();
}

void NoiseChannel::reset()
{
	enabled = false;
	dacEnabled = false;

	nr41 = 0;
	nr42 = 0;
	nr43 = 0;
	nr44 = 0;

	lengthCounter = 0;
	frequencyTimer = 0;

	lfsr = 0x7FFF;

	currentVolume = 0;
	envelopeTimer = 0;
}

uint32_t NoiseChannel::getTimerPeriod() const
{
	static constexpr uint32_t divisors[8] =
	{
		8, 16, 32, 48, 64, 80, 96, 112
	};

	uint8_t divisorCode = nr43 & 0x07;
	uint8_t shift = nr43 >> 4;

	return divisors[divisorCode] << shift;
}

void NoiseChannel::tick()
{
	if (frequencyTimer > 0)
		frequencyTimer--;

	if (frequencyTimer != 0)
		return;

	frequencyTimer = getTimerPeriod();

	uint8_t xorResult =
		(lfsr & 0x01) ^ ((lfsr >> 1) & 0x01);

	lfsr >>= 1;

	// Insert feedback into bit 14.
	lfsr |= static_cast<uint16_t>(xorResult) << 14;

	// NR43 bit 3 selects 7-bit LFSR mode.
	if (nr43 & 0x08)
	{
		lfsr &= ~(1 << 6);
		lfsr |= static_cast<uint16_t>(xorResult) << 6;
	}
}

void NoiseChannel::clockLength()
{
	if (!(nr44 & 0x40))
		return;

	if (lengthCounter == 0)
		return;

	lengthCounter--;

	if (lengthCounter == 0)
		enabled = false;
}

void NoiseChannel::clockEnvelope()
{
	if (!enabled)
		return;

	uint8_t period = nr42 & 0x07;

	if (period == 0)
		return;

	if (envelopeTimer > 0)
		envelopeTimer--;

	if (envelopeTimer != 0)
		return;

	envelopeTimer = period;

	bool increase = (nr42 & 0x08) != 0;

	if (increase)
	{
		if (currentVolume < 15)
			currentVolume++;
	}
	else
	{
		if (currentVolume > 0)
			currentVolume--;
	}
}

void NoiseChannel::trigger()
{
	enabled = dacEnabled;

	if (lengthCounter == 0)
		lengthCounter = 64;

	frequencyTimer = getTimerPeriod();

	lfsr = 0x7FFF;

	currentVolume = nr42 >> 4;

	envelopeTimer = nr42 & 0x07;

	if (envelopeTimer == 0)
		envelopeTimer = 8;
}

uint8_t NoiseChannel::read(uint16_t address) const
{
	switch (address)
	{
	case 0xFF20:
		return 0xFF;

	case 0xFF21:
		return nr42;

	case 0xFF22:
		return nr43;

	case 0xFF23:
		return nr44 | 0xBF;
	}

	return 0xFF;
}

void NoiseChannel::write(uint16_t address, uint8_t value)
{
	switch (address)
	{
	case 0xFF20:
		nr41 = value;
		lengthCounter = 64 - (value & 0x3F);
		break;

	case 0xFF21:
		nr42 = value;

		dacEnabled = (value & 0xF8) != 0;

		if (!dacEnabled)
			enabled = false;
		break;

	case 0xFF22:
		nr43 = value;
		break;

	case 0xFF23:
		nr44 = value;

		if (value & 0x80)
			trigger();
		break;
	}
}

float NoiseChannel::getSample() const
{
	if (!enabled || !dacEnabled)
		return 0.0f;

	bool output = !(lfsr & 0x01);

	if (!output)
		return 0.0f;

	return static_cast<float>(currentVolume) / 15.0f;
}

bool NoiseChannel::isEnabled() const
{
	return enabled;
}