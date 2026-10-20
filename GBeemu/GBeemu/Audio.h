#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

class Audio
{
public:
	Audio();
	~Audio();

	Audio(const Audio&) = delete;
	Audio& operator=(const Audio&) = delete;

	Audio(Audio&&) = delete;
	Audio& operator=(Audio&&) = delete;

	bool initialize();
	void shutdown();

	void queueSamples(
		const float* samples,
		uint32_t count
	);

	void playTestTone();

	void clear();

	uint32_t getQueuedBytes() const;

private:
	SDL_AudioStream* stream = nullptr;

	static constexpr int SAMPLE_RATE = 48000;
	static constexpr int CHANNELS = 2;
};
