
#include "SquareChannel.h"

SquareChannel::SquareChannel(
	uint16_t baseAddress,
	bool hasSweep
)
	: baseAddress(baseAddress), hasSweep(hasSweep)
{
	reset();
}

void SquareChannel::reset()
{
	enabled = false;
	dacEnabled = false;

	nr10 = 0;
	nr21 = 0;
	nr22 = 0;
	nr23 = 0;
	nr24 = 0;

	dutyPosition = 0;
	frequencyTimer = 0;
	lengthCounter = 0;

	currentVolume = 0;
	envelopeTimer = 0;

	shadowFrequency = 0;
	sweepTimer = 0;
	sweepEnabled = false;
	sweepNegateUsed = false;
}

void SquareChannel::tick()
{
	if (frequencyTimer > 0)
		frequencyTimer--;

	if (frequencyTimer == 0)
	{
		uint16_t frequency =
			((nr24 & 0x07) << 8) | nr23;

		frequencyTimer =
			(2048 - frequency) * 4;

		dutyPosition =
			(dutyPosition + 1) & 0x07;
	}
}

void SquareChannel::clockLength()
{
	if (!(nr24 & 0x40))
		return;

	if (lengthCounter == 0)
		return;

	lengthCounter--;

	if (lengthCounter == 0)
		enabled = false;
}

void SquareChannel::clockEnvelope()
{
	if (!enabled)
		return;

	uint8_t period = nr22 & 0x07;

	if (period == 0)
		return;

	if (envelopeTimer > 0)
		envelopeTimer--;

	if (envelopeTimer != 0)
		return;

	envelopeTimer = period;

	bool increase = (nr22 & 0x08) != 0;

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

void SquareChannel::trigger()
{
	dacEnabled = (nr22 & 0xF8) != 0;
	enabled = dacEnabled;

	if (lengthCounter == 0)
		lengthCounter = 64;

	uint16_t frequency =
		((nr24 & 0x07) << 8) | nr23;

	frequencyTimer =
		(2048 - frequency) * 4;

	currentVolume = nr22 >> 4;

	envelopeTimer = nr22 & 0x07;

	if (envelopeTimer == 0)
		envelopeTimer = 8;

	if (hasSweep)
	{
		shadowFrequency =
			((nr24 & 0x07) << 8) | nr23;

		uint8_t period = (nr10 >> 4) & 0x07;
		uint8_t shift = nr10 & 0x07;

		sweepTimer = period == 0 ? 8 : period;

		sweepEnabled = (period != 0 || shift != 0);

		sweepNegateUsed = false;

		if (shift != 0)
		{
			calculateSweepFrequency();
		}
	}
}

uint8_t SquareChannel::read(uint16_t address) const
{
	if (hasSweep && address == 0xFF10)
		return nr10 | 0x80;

	switch (address - baseAddress)
	{
	case 0:
		return nr21 | 0x3F;

	case 1:
		return nr22;

	case 2:
		return 0xFF;

	case 3:
		return nr24 | 0xBF;
	}

	return 0xFF;
}

void SquareChannel::write(uint16_t address, uint8_t value)
{
	if (hasSweep && address == 0xFF10)
	{
		bool oldNegate = (nr10 & 0x08) != 0;
		bool newNegate = (value & 0x08) != 0;

		if (sweepNegateUsed && oldNegate && !newNegate)
			enabled = false;

		nr10 = value & 0x7F;
		return;
	}

	switch (address - baseAddress)
	{
	case 0:
		nr21 = value;

		lengthCounter = 64 - (value & 0x3F);
		break;

	case 1:
		nr22 = value;

		dacEnabled = (value & 0xF8) != 0;

		if (!dacEnabled)
			enabled = false;
		break;

	case 2:
		nr23 = value;
		break;

	case 3:
		nr24 = value;

		if (value & 0x80)
			trigger();
		break;
	}
}

float SquareChannel::getSample() const
{
	if (!enabled || !dacEnabled)
		return 0.0f;

	uint8_t duty = nr21 >> 6;

	uint8_t output =
		DUTY_TABLE[duty][dutyPosition];

	if (output == 0)
		return 0.0f;

	return static_cast<float>(currentVolume) / 15.0f;
}

bool SquareChannel::isEnabled() const
{
	return enabled;
}

uint16_t SquareChannel::calculateSweepFrequency()
{
	uint8_t shift = nr10 & 0x07;

	uint16_t delta = shadowFrequency >> shift;
	uint16_t newFrequency;

	if (nr10 & 0x08)
	{
		newFrequency = shadowFrequency - delta;
		sweepNegateUsed = true;
	}
	else
	{
		newFrequency = shadowFrequency + delta;
	}

	if (newFrequency > 2047)
	{
		enabled = false;
	}

	return newFrequency;
}

void SquareChannel::clockSweep()
{
	if (!hasSweep || !sweepEnabled)
		return;

	if (sweepTimer > 0)
		sweepTimer--;

	if (sweepTimer != 0)
		return;

	uint8_t period = (nr10 >> 4) & 0x07;

	sweepTimer = period == 0 ? 8 : period;

	if (period == 0)
		return;

	uint16_t newFrequency = calculateSweepFrequency();

	if (!enabled)
		return;

	uint8_t shift = nr10 & 0x07;

	if (shift != 0)
	{
		shadowFrequency = newFrequency;

		nr23 = newFrequency & 0xFF;
		nr24 = (nr24 & 0xF8) |
			((newFrequency >> 8) & 0x07);

		// Check whether the next calculation would overflow.
		calculateSweepFrequency();
	}
}