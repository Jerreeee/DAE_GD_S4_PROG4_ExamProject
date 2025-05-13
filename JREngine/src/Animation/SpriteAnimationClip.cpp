#include "JREngine/Core/Timer.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

namespace JRE
{
    SpriteAnimationClip::SpriteAnimationClip(const std::vector<JRE::SoftAssetRef<Sprite>>& sprites, int framesPerSec)
    {
        SetSprites(sprites, framesPerSec);
    }
    void SpriteAnimationClip::Update()
    {
        auto dt = Timer::GetInstance().GetDeltaTime();
        m_AccTime += dt;
        if (m_AccTime > m_TimePerFrame)
        {
            m_AccTime -= m_TimePerFrame;
            ++m_CurFrameIdx;
            if (m_CurFrameIdx > m_Sprites.size() - 1) //reset to start
            {
                EventInfo e = CreateEvent<Events::EndOfClipEvent>();
                OnEndOfClipEvent.Notify(e);
                ResetToStart();
            }
        }
    }
    void SpriteAnimationClip::ResetToStart()
    {
        m_CurFrameIdx = 0;
    }
    void SpriteAnimationClip::SetSprites(const std::vector<SoftAssetRef<Sprite>>& sprites, int framesPerSec)
    {
        m_Sprites = sprites;
        m_FramesPerSec = framesPerSec;
        m_TimePerFrame = 1.f / m_FramesPerSec;
        ResetToStart();
    }
    AssetRef<Sprite> SpriteAnimationClip::GetCurrentSprite() const
    {
        return m_Sprites[m_CurFrameIdx].Get();
    }
}
