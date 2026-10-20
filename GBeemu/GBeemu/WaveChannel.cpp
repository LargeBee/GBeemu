
#include "WaveChannel.h"

WaveChannel::WaveChannel()
{
	reset();
}

void WaveChannel::reset()
{
	powerOff();

	waveRAM.fill(0);
}

void WaveChannel::tick()
{
	if (frequencyTimer > 0)
		frequencyTimer--;

	if (frequencyTimer == 0)
	{
		uint16_t frequency =
			((nr34 & 0x07) << 8) | nr33;

		frequencyTimer = (2048 - frequency) * 2;

		samplePosition = (samplePosition + 1) & 0x1F;

		sampleBuffer = waveRAM[samplePosition / 2];
	}
}

void WaveChannel::clockLength()
{
	if (!(nr34 & 0x40))
		return;

	if (lengthCounter == 0)
		return;

	lengthCounter--;

	if (lengthCounter == 0)
		enabled = false;
}

void WaveChannel::trigger()
{
	enabled = dacEnabled;

	if (lengthCounter == 0)
		lengthCounter = 256;

	uint16_t frequency =
		((nr34 & 0x07) << 8) | nr33;

	frequencyTimer = (2048 - frequency) * 2;

	samplePosition = 0;
	sampleBuffer = waveRAM[0];
}

uint8_t WaveChannel::read(uint16_t address) const
{
	if (address >= 0xFF30 && address <= 0xFF3F)
	{
		return waveRAM[address - 0xFF30];
	}

	switch (address)
	{
	case 0xFF1A:
		return nr30 | 0x7F;

	case 0xFF1B:
		return 0xFF;

	case 0xFF1C:
		return nr32 | 0x9F;

	case 0xFF1D:
		return 0xFF;

	case 0xFF1E:
		return nr34 | 0xBF;
	}

	return 0xFF;
}

void WaveChannel::write(uint16_t address, uint8_t value)
{
	if (address >= 0xFF30 && address <= 0xFF3F)
	{
		waveRAM[address - 0xFF30] = value;
		return;
	}

	switch (address)
	{
	case 0xFF1A:
		nr30 = value;

		dacEnabled = (value & 0x80) != 0;

		if (!dacEnabled)
			enabled = false;
		break;

	case 0xFF1B:
		nr31 = value;
		lengthCounter = 256 - value;
		break;

	case 0xFF1C:
		nr32 = value;
		break;

	case 0xFF1D:
		nr33 = value;
		break;

	case 0xFF1E:
		nr34 = value;

		if (value & 0x80)
			trigger();
		break;
	}
}

float WaveChannel::getSample() const
{
	if (!enabled || !dacEnabled)
		return 0.0f;

	uint8_t outputLevel = (nr32 >> 5) & 0x03;

	if (outputLevel == 0)
		return 0.0f;

	uint8_t sample;

	if ((samplePosition & 1) == 0)
		sample = sampleBuffer >> 4;
	else
		sample = sampleBuffer & 0x0F;

	sample >>= (outputLevel - 1);

	return static_cast<float>(sample) / 15.0f;
}

bool WaveChannel::isEnabled() const
{
	return enabled;
}

void WaveChannel::powerOff()
{
	enabled = false;
	dacEnabled = false;

	nr30 = 0;
	nr31 = 0;
	nr32 = 0;
	nr33 = 0;
	nr34 = 0;

	frequencyTimer = 0;
	lengthCounter = 0;

	samplePosition = 0;
	sampleBuffer = 0;
}