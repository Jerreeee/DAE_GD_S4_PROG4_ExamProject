#pragma once
#include "JREngine/Resources/Asset.h"
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	using SoundID = unsigned short;
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void Play(Ref<ISoundClip> clip, float volume = 1.0f) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		virtual void Play(Ref<ISoundClip>, float) override {};
	};
}
