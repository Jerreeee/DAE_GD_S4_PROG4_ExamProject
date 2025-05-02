#pragma once
#include <memory>
#include <string>
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	class SDLSoundClip : public ISoundClip
	{
	public:
		explicit SDLSoundClip(const std::string& filePath);
		~SDLSoundClip();

		SDLSoundClip(const SDLSoundClip&) = delete;
		SDLSoundClip& operator=(const SDLSoundClip&) = delete;
		SDLSoundClip(SDLSoundClip&& other) noexcept;
		SDLSoundClip& operator=(SDLSoundClip&& other) noexcept;

		void Play(int loops = 0, int channel = -1) const;
		void SetVolume(float volume);
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}