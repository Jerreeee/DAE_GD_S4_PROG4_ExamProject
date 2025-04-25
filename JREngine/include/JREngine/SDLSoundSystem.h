#pragma once
#include <memory>
#include "ISoundSystem.h"

namespace JREngine
{
	using SoundHandle = uint16_t;
	class SoundClip;
	class SDLSoundSystem final : public ISoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem();

		virtual void Play(std::shared_ptr<SoundClip> clip, float volume = 1.0f) override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}