#pragma once
#include <vector>
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Core/Event.h"

namespace JRE
{
	class SpriteAnimationClip;
	namespace Events
	{
		struct EndOfClipEvent
		{
			static const EventID ID{ HashEventID("EndOfClip") };
			struct Args : public EventArgs
			{
				Args(SpriteAnimationClip* _clip) : clip{ _clip } {}
				SpriteAnimationClip* clip;
			};
		};
	}

	class SpriteAnimationClip final : public Asset
	{
	public:
		REGISTER_TYPE_HEADER(Asset);

		SpriteAnimationClip(const std::vector<SoftAssetRef<Sprite>>& sprites, int framesPerSec, bool isPong = false);

		void Update();
		void ResetToStart();

		AssetRef<Sprite> GetCurrentSprite() const;

		Event OnEndOfClipEvent{};
	private:
		std::vector<SoftAssetRef<Sprite>> m_Sprites{};
		size_t m_CurFrameIdx{};
		int m_FramesPerSec{};
		float m_TimePerFrame{};
		float m_AccTime{};
		bool m_IsPong{};
		int m_PlaybackDirection{ 1 };
	};
}