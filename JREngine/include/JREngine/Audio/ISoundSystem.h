#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/AssetMetadata.h"
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	using SoundID = unsigned short;
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual AssetRef<ISoundClip> CreateSoundClip(AssetHandle handle, const AssetMetadata& metadata) = 0;
		virtual void Play(AssetRef<ISoundClip> soundClipAsset, float volume = 1.0f) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		virtual AssetRef<ISoundClip> CreateSoundClip(AssetHandle, const AssetMetadata&) override { return AssetRef<NullSoundClip>{}; };
		virtual void Play(AssetRef<ISoundClip>, float) override {};
	};
}
