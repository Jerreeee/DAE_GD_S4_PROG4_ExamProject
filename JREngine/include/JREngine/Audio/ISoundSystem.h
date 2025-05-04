#pragma once
#include <memory>
#include <string>
#include "JREngine/Resources/Asset.h"
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	using SoundID = unsigned short;
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual Ref<ISoundClip> CreateSoundClip(const std::string& path, AssetHandle handle = AssetHandle::InvalidUUID) = 0;
		virtual void Play(Ref<ISoundClip> clip, float volume = 1.0f) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		Ref<ISoundClip> CreateSoundClip(const std::string&, AssetHandle) override { return std::make_shared<NullSoundClip>(); }
		virtual void Play(Ref<ISoundClip>, float) override {};
	};
}