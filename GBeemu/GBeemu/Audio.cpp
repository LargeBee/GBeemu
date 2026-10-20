
#include "Audio.h"
#include <cmath>
#include <vector>

Audio::Audio()
{
}

Audio::~Audio()
{
	shutdown();
}

bool Audio::initialize()
{
	if (stream != nullptr)
		return true;

	SDL_AudioSpec spec{};

	spec.freq = SAMPLE_RATE;
	spec.format = SDL_AUDIO_F32;
	spec.channels = CHANNELS;

	stream = SDL_OpenAudioDeviceStream(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
		&spec,
		nullptr,
		nullptr
	);

	if (stream == nullptr)
	{
		SDL_Log(
			"Failed to open audio: %s",
			SDL_GetError()
		);

		return false;
	}

	if (!SDL_ResumeAudioStreamDevice(stream))
	{
		SDL_Log(
			"Failed to start audio: %s",
			SDL_GetError()
		);

		shutdown();
		return false;
	}

	return true;
}

void Audio::shutdown()
{
	if (stream == nullptr)
		return;

	SDL_DestroyAudioStream(stream);
	stream = nullptr;
}

void Audio::queueSamples(
	const float* samples,
	uint32_t count
)
{
	if (stream == nullptr || samples == nullptr || count == 0)
		return;

	constexpr uint32_t MAX_QUEUED_BYTES = 38400;

	if (getQueuedBytes() >= MAX_QUEUED_BYTES)
		return;

	SDL_PutAudioStreamData(
		stream,
		samples,
		static_cast<int>(count * sizeof(float))
	);
}

void Audio::clear()
{
	if (stream == nullptr)
		return;

	SDL_ClearAudioStream(stream);
}

uint32_t Audio::getQueuedBytes() const
{
	if (stream == nullptr)
		return 0;

	int bytes = SDL_GetAudioStreamQueued(stream);

	return bytes > 0
		? static_cast<uint32_t>(bytes)
		: 0;
}

void Audio::playTestTone()
{
	constexpr int duration = 2;
	constexpr float frequency = 440.0f;
	constexpr float volume = 0.15f;
	constexpr float pi = 3.14159265358979323846f;

	std::vector<float> samples(
		SAMPLE_RATE * duration * CHANNELS
	);

	for (int i = 0; i < SAMPLE_RATE * duration; i++)
	{
		float time =
			static_cast<float>(i) / SAMPLE_RATE;

		float sample =
			std::sin(2.0f * pi * frequency * time) *
			volume;

		samples[i * 2] = sample;
		samples[i * 2 + 1] = sample;
	}

	queueSamples(
		samples.data(),
		static_cast<uint32_t>(samples.size())
	);
}