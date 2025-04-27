#pragma once
#include <memory>

namespace JRE
{
	using SoundID = unsigned short;
	class SoundClip;
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void Play(std::shared_ptr<SoundClip> clip, float volume = 1.0f) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		virtual void Play(std::shared_ptr<SoundClip> /*clip*/, float /*volume*/ = 1.0f) override {};
	};
}