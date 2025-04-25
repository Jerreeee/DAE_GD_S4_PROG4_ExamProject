#include <iostream>
#include "SDL_mixer.h"
#include "SoundClip.h"

namespace JREngine
{
	SoundClip::SoundClip(const std::string& filePath)
	{
		m_pChunk = Mix_LoadWAV(filePath.c_str());
		if (!m_pChunk)
			std::cerr << "Failed to load sound: " << filePath << " - " << Mix_GetError() << "\n";
	}

	SoundClip::~SoundClip()
	{
		if (m_pChunk)
			Mix_FreeChunk(m_pChunk);
	}

	SoundClip::SoundClip(SoundClip&& other) noexcept :
		m_pChunk(other.m_pChunk)
	{
		other.m_pChunk = nullptr;
	}

	SoundClip& SoundClip::operator=(SoundClip&& other) noexcept
	{
		if (this != &other)
		{
			if (m_pChunk)
				Mix_FreeChunk(m_pChunk);

			m_pChunk = other.m_pChunk;
			other.m_pChunk = nullptr;
		}
		return *this;
	}

	void SoundClip::Play(int loops, int channel) const
	{
		if (m_pChunk)
			Mix_PlayChannel(channel, m_pChunk, loops);
	}

	void SoundClip::SetVolume(float volume)
	{
		if (m_pChunk)
			Mix_VolumeChunk(m_pChunk, static_cast<int>(volume * MIX_MAX_VOLUME));
	}
}