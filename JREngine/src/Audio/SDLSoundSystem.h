#pragma once
#include <memory>
#include "JREngine/Audio/ISoundSystem.h"

namespace JRE
{
	using SoundHandle = uint16_t;
	class SDLSoundSystem final : public ISoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem();

		virtual Ref<ISoundClip> CreateSoundClip(const std::string& path, AssetHandle handle = AssetHandle::InvalidUUID) override;
		virtual void Play(Ref<ISoundClip> clip, float volume = 1.0f) override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}