#pragma once
#include "JREngine/Resources/Asset.h"
#include "JREngine/Resources/AssetMetadata.h"
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	using SoundID = unsigned short;
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual Ref<ISoundClip> CreateSoundClip(AssetHandle handle, const AssetMetadata& metadata) = 0;
		virtual void Play(Ref<ISoundClip> clip, float volume = 1.0f) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		virtual Ref<ISoundClip> CreateSoundClip(AssetHandle, const AssetMetadata&) override { return Ref<NullSoundClip>{}; };
		virtual void Play(Ref<ISoundClip>, float) override {};
	};
}
