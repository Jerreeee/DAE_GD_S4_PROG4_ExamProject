#include "JREngine/Core/Timer.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

namespace JRE
{
	REGISTER_TYPE_SOURCE(SpriteAnimationClip, JRE::Asset, "JRE::SpriteAnimationClip");

    SpriteAnimationClip::SpriteAnimationClip(const std::vector<JRE::SoftAssetRef<Sprite>>& sprites, int framesPerSec, bool isPong)
        : m_Sprites{ sprites }
        , m_FramesPerSec{ framesPerSec }
        , m_TimePerFrame{ 1.f / m_FramesPerSec }
        , m_IsPong{ isPong }
    {
        ResetToStart();
    }
    void SpriteAnimationClip::Update()
    {
		auto dt = Timer::GetInstance().GetDeltaTime();
		m_AccTime += dt;

		if (m_AccTime > m_TimePerFrame)
		{
			m_AccTime -= m_TimePerFrame;
			m_CurFrameIdx += m_PlaybackDirection;

			if (m_IsPong) //Reverse direction at end
			{
				if (m_CurFrameIdx >= m_Sprites.size())
				{
					m_CurFrameIdx = m_Sprites.size() - 2;
					m_PlaybackDirection = -1;
					EventInfo e = CreateEvent<Events::EndOfClipEvent>(this);
					OnEndOfClipEvent.Notify(e);
				}
				else if (m_CurFrameIdx < 0)
				{
					m_CurFrameIdx = 1;
					m_PlaybackDirection = 1;
					EventInfo e = CreateEvent<Events::EndOfClipEvent>(this);
					OnEndOfClipEvent.Notify(e);
				}
			}
			else //Loop around
			{
				if (m_CurFrameIdx >= m_Sprites.size())
				{
					EventInfo e = CreateEvent<Events::EndOfClipEvent>(this);
					OnEndOfClipEvent.Notify(e);
					ResetToStart();
				}
			}
		}
    }
    void SpriteAnimationClip::ResetToStart()
    {
        m_CurFrameIdx = 0;
        m_PlaybackDirection = 1;
    }
    AssetRef<Sprite> SpriteAnimationClip::GetCurrentSprite() const
    {
        return m_Sprites[m_CurFrameIdx].Get();
    }
}
