#include "APU.h"
#include "Bus.h"

APU::APU()
{
	bus = nullptr;

	reset();
}

void APU::setBus(Bus& bus)
{
	this->bus = &bus;
}

void APU::reset()
{
	channel1.reset();
	channel2.reset();
	channel3.reset();
	channel4.reset();

	nr50 = 0;
	nr51 = 0;
	nr52 = 0;

	frameSequencerStep = 0;

	sampleCounter = 0;
	sampleBuffer.clear();
}

void APU::tick()
{
	if (nr52 & 0x80)
	{
		channel1.tick();
		channel2.tick();
		channel3.tick();
		channel4.tick();
	}

	sampleCounter += SAMPLE_RATE;

	if (sampleCounter >= GAMEBOY_CLOCK)
	{
		sampleCounter -= GAMEBOY_CLOCK;

		float sample1 = channel1.getSample();
		float sample2 = channel2.getSample();
		float sample3 = channel3.getSample();
		float sample4 = channel4.getSample();

		float left = 0.0f;
		float right = 0.0f;

		if (nr52 & 0x80)
		{
			// Channel 1
			if (nr51 & 0x01)
				right += sample1;

			if (nr51 & 0x10)
				left += sample1;

			// Channel 2
			if (nr51 & 0x02)
				right += sample2;

			if (nr51 & 0x20)
				left += sample2;

			// Channel 3
			if (nr51 & 0x04)
				right += sample3;

			if (nr51 & 0x40)
				left += sample3;

			// Channel 4
			if (nr51 & 0x08)
				right += sample4;

			if (nr51 & 0x80)
				left += sample4;
		}

		float leftVolume =
			static_cast<float>(((nr50 >> 4) & 0x07) + 1) / 8.0f;

		float rightVolume =
			static_cast<float>((nr50 & 0x07) + 1) / 8.0f;

		// Four channels can now contribute to the mix.
		left *= leftVolume * 0.04f;
		right *= rightVolume * 0.04f;

		sampleBuffer.push_back(left);
		sampleBuffer.push_back(right);
	}
}

uint8_t APU::read(uint16_t address)
{
	if (address >= 0xFF10 && address <= 0xFF14)
		return channel1.read(address);

	if (address >= 0xFF16 && address <= 0xFF19)
		return channel2.read(address);

	if (address >= 0xFF20 && address <= 0xFF23)
		return channel4.read(address);

	if (address >= 0xFF1A && address <= 0xFF1E)
		return channel3.read(address);

	if (address >= 0xFF30 && address <= 0xFF3F)
		return channel3.read(address);

	switch (address)
	{
	case 0xFF24:
		return nr50;

	case 0xFF25:
		return nr51;

	case 0xFF26:
		return (nr52 & 0x80) |
			(channel1.isEnabled() ? 0x01 : 0x00) |
			(channel2.isEnabled() ? 0x02 : 0x00) |
			(channel3.isEnabled() ? 0x04 : 0x00) |
			(channel4.isEnabled() ? 0x08 : 0x00) |
			0x70;
	}

	return 0xFF;
}

void APU::write(uint16_t address, uint8_t value)
{
	if (address >= 0xFF30 && address <= 0xFF3F)
	{
		channel3.write(address, value);
		return;
	}

	if (address == 0xFF26)
	{
		bool powerOn = (value & 0x80) != 0;
		bool wasPowered = (nr52 & 0x80) != 0;

		if (!powerOn)
		{
			nr52 = 0;
			nr50 = 0;
			nr51 = 0;

			channel1.reset();
			channel2.reset();
			channel3.powerOff();
			channel4.reset();

			frameSequencerStep = 0;
		}
		else if (!wasPowered)
		{
			nr52 = 0x80;

			frameSequencerStep = 0;
		}

		return;
	}

	if (!(nr52 & 0x80))
		return;

	if (address >= 0xFF10 && address <= 0xFF14)
	{
		channel1.write(address, value);
		return;
	}

	if (address >= 0xFF16 && address <= 0xFF19)
	{
		channel2.write(address, value);
		return;
	}

	if (address >= 0xFF20 && address <= 0xFF23)
	{
		channel4.write(address, value);
		return;
	}

	if (address >= 0xFF1A && address <= 0xFF1E)
	{
		channel3.write(address, value);
		return;
	}

	switch (address)
	{
	case 0xFF24:
		nr50 = value;
		break;

	case 0xFF25:
		nr51 = value;
		break;
	}
}

float APU::generateSample()
{
	static float phase = 0.0f;

	constexpr float frequency = 440.0f;
	constexpr float volume = 0.1f;

	phase += frequency / SAMPLE_RATE;

	if (phase >= 1.0f)
		phase -= 1.0f;

	return phase < 0.5f
		? volume
		: -volume;
}

const std::vector<float>& APU::getSampleBuffer() const
{
	return sampleBuffer;
}

void APU::clearSampleBuffer()
{
	sampleBuffer.clear();
}

void APU::clockFrameSequencer()
{
	if (!(nr52 & 0x80))
		return;

	switch (frameSequencerStep)
	{
	case 0:
	case 2:
	case 4:
	case 6:
		channel1.clockLength();
		channel2.clockLength();
		channel3.clockLength();
		channel4.clockLength();
		break;
	}

	if (frameSequencerStep == 2 ||
		frameSequencerStep == 6)
	{
		channel1.clockSweep();
	}

	if (frameSequencerStep == 7)
	{
		channel1.clockEnvelope();
		channel2.clockEnvelope();
		channel4.clockEnvelope();
	}

	frameSequencerStep =
		(frameSequencerStep + 1) & 0x07;
}