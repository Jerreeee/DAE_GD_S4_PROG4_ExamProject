#pragma once
#include <vector>
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Core/Event.h"

namespace JRE
{
	namespace Events
	{
		struct EndOfClipEvent
		{
			static const EventID ID{ HashEventID("EndOfClip") };
			struct Args : public EventArgs
			{
				Args() = default;
			};
		};
	}

	class SpriteAnimationClip final : public Asset
	{
	public:
		SpriteAnimationClip(const std::vector<SoftAssetRef<Sprite>>& sprites, int framesPerSec);

		void Update();
		void ResetToStart();

		void SetSprites(const std::vector<SoftAssetRef<Sprite>>& sprites, int framesPerSec);
		AssetRef<Sprite> GetCurrentSprite() const;

		static constexpr std::string_view GetStaticType() { return "SpriteAnimationClip"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };

		Event OnEndOfClipEvent{};
	private:
		std::vector<SoftAssetRef<Sprite>> m_Sprites{};
		int m_CurFrameIdx{};
		int m_FramesPerSec{};
		float m_TimePerFrame{};
		float m_AccTime{};
	};
}