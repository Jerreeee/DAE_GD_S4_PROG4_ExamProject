#pragma once
#include <vector>
#include "JREngine/Asset/Asset.h"
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
		SpriteAnimationClip(const std::vector<AssetHandle>& spriteHandles, int framesPerSec, bool isPong = false);

		void Update();
		void ResetToStart();

		AssetHandle GetCurrentSpriteHandle() const;

		static constexpr std::string_view GetStaticType() { return "SpriteAnimationClip"; };
		virtual std::string_view GetType() const override { return GetStaticType(); };

		Event OnEndOfClipEvent{};
	private:
		std::vector<AssetHandle> m_SpriteHandles{};
		size_t m_CurFrameIdx{};
		int m_FramesPerSec{};
		float m_TimePerFrame{};
		float m_AccTime{};
		bool m_IsPong{};
		int m_PlaybackDirection{ 1 };
	};
}