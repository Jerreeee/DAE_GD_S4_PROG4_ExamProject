#pragma once
#include <memory>
#include <string>
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	using SoundID = unsigned short;
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual std::shared_ptr<ISoundClip> CreateSoundClip(const std::string& path) = 0;
		virtual void Play(std::shared_ptr<ISoundClip> clip, float volume = 1.0f) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		std::shared_ptr<ISoundClip> CreateSoundClip(const std::string&) override { return std::make_shared<NullSoundClip>(); }
		virtual void Play(std::shared_ptr<ISoundClip>, float) override {};
	};
}