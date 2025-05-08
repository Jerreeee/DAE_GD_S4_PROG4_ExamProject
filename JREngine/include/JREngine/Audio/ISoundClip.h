#pragma once
#include "JREngine/Resources/Asset.h"

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
	};

	class NullSoundClip final : public ISoundClip
	{
	public:
		void Play(int, int) const override {}
		void SetVolume(float) override {}
	};
}