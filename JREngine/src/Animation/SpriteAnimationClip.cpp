#include "JREngine/Core/Timer.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

namespace JRE
{
    SpriteAnimationClip::SpriteAnimationClip(const std::vector<AssetHandle>& spriteHandles, int framesPerSec, bool isPong)
        : m_SpriteHandles{ spriteHandles }
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
				if (m_CurFrameIdx >= m_SpriteHandles.size())
				{
					m_CurFrameIdx = m_SpriteHandles.size() - 2;
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
				if (m_CurFrameIdx >= m_SpriteHandles.size())
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
    AssetHandle SpriteAnimationClip::GetCurrentSpriteHandle() const
    {
        return m_SpriteHandles[m_CurFrameIdx];
    }
}
