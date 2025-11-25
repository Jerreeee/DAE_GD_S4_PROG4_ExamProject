#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Audio/ISoundSystem.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Core/Event.h"

#include "PlayerStates.h"
#include "PlayerScriptComponent.h"
#include "EngineSetup.h"

using namespace JRE;

namespace BubbleBobble
{
    void PlayerState_Mortal::Update()
    {
        m_Player->MoveUpdate();
    }

    void PlayerState_Mortal::FixedUpdate()
    {
        m_Player->MoveCollider();
    }

    void PlayerState_Mortal::OnEvent(JRE::EventInfo& event)
    {
        switch (event.GetID())
        {
        case JRE::Events::Box2DCollisionEvent::ID:
        {
            auto& args = event.GetArgs<JRE::Events::Box2DCollisionEvent>();

            if (args.other.GetProperties().layer & CollisionLayer::Enemy)
                m_Player->RequestStateChange(PlayerState::Dead);
            break;
        }
        case JRE::Events::EndOfClipEvent::ID:
        {
            auto& args = event.GetArgs<JRE::Events::EndOfClipEvent>();

            if (args.clip == m_Player->m_pShootClipRef.get())
                m_Player->m_AnimState = PlayerScriptComponent::AnimState::Moving;
            break;
        }
        }
    }

    void PlayerState_Immortal::Update()
    {
        float dt = Timer::GetInstance().GetDeltaTime();
        m_Time -= dt;

        m_Player->MoveUpdate();

        if (m_Time <= 0.f)
        {
            m_Player->RequestStateChange(PlayerState::Mortal);
            m_Time = m_TimeDefault;
        }
    }

    void PlayerState_Immortal::FixedUpdate()
    {
        m_Player->MoveCollider();
    }

    void PlayerState_Dead::OnEnter()
    {
        m_Player->m_pSpriteAnimatorCmp->SetActiveClip("Death");
        m_Player->m_pSpriteRendererCmp->SetOffset(glm::vec2(0, -45));
    }

    void PlayerState_Dead::OnEvent(JRE::EventInfo& event)
    {
        if (event.GetID() == JRE::Events::EndOfClipEvent::ID)
        {
            auto& args = event.GetArgs<JRE::Events::EndOfClipEvent>();

            if (args.clip == m_Player->m_pDeathClipRef.get())
            {
                m_Player->m_pHealthCmp->TakeDamage(1);

                JRE::EventInfo e = CreateEvent<Events::PlayerLostLive>(m_Player->m_pHealthCmp->GetHealth());
                m_Player->OnPlayerLostLive.Notify(e);

                m_Player->m_pSpriteRendererCmp->SetOffset();
                m_Player->m_AnimState = PlayerScriptComponent::AnimState::Moving;
                m_Player->RequestStateChange(PlayerState::Immortal);
            }
        }
    }
}
