#pragma once
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	class ISoundClip : public Asset
	{
	public:
		ISoundClip() = default;
		ISoundClip(AssetHandle handle) : Asset(handle) {};
		virtual ~ISoundClip() = default;

		virtual void Play(int loops = 0, int channel = -1) const = 0;
		virtual void SetVolume(float volume) = 0;

		static constexpr std::string_view GetStaticType() { return "SoundClip"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };
	};

	class NullSoundClip final : public ISoundClip
	{
	public:
		void Play(int, int) const override {}
		void SetVolume(float) override {}
	};
}