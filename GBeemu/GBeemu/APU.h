#pragma once

#include <vector>
#include <cstdint>

#include "SquareChannel.h"
#include "WaveChannel.h"
#include "NoiseChannel.h"

class Bus;

class APU
{
public:
	APU();

	void setBus(Bus& bus);

	void reset();
	void tick();

	const std::vector<float>& getSampleBuffer() const;
	void clearSampleBuffer();

	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	void clockFrameSequencer();

private:
	Bus* bus;

	SquareChannel channel1{ 0xFF11, true };
	SquareChannel channel2{ 0xFF16, false };
	WaveChannel channel3;
	NoiseChannel channel4;

	uint8_t nr50 = 0;
	uint8_t nr51 = 0;
	uint8_t nr52 = 0;

	uint8_t frameSequencerStep = 0;

	uint8_t registers[0x30];

	static constexpr uint32_t GAMEBOY_CLOCK = 4194304;
	static constexpr uint32_t SAMPLE_RATE = 48000;

	uint32_t sampleCounter = 0;

	std::vector<float> sampleBuffer;

	float generateSample();
};