#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/Timer.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Rendering/SDLRenderer.h"
#include "JREngine/Physics/Box2DColliderComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Animation/SpriteAnimationClip.h"

#include "EngineSetup.h"
#include "Enemies/Zenchan/ZenchanScriptComponent.h"
#include "Bubble/BubbleScript.h"

#include <iostream>

using namespace JRE;

namespace BubbleBobble
{
	BubbleScript::BubbleScript(GameObject& gameObject, int direction)
		: ComponentBase(gameObject),
		m_Direction(direction)
	{
		m_Vel.x = direction * m_SpeedX;

		m_pBox2ColliderCmp = GetGameObject().GetComponent<Box2DColliderComponent>();
		assert(m_pBox2ColliderCmp && "BubbleScript doesnt ahve Box2DColliderComponent");
		m_pSpriteAnimCmp = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		assert(m_pSpriteAnimCmp && "BubbleScript doesnt ahve SpriteAnimatorComponent");

		m_pBox2ColliderCmp->OnCollisionEvent.AddObserver(this);

		m_pPoofAnimClip = m_pSpriteAnimCmp->GetClip("Poof").get();
		m_pPoofAnimClip->OnEndOfClipEvent.AddObserver(this);
	}

	void BubbleScript::Update()
	{
		float dt = Timer::GetInstance().GetDeltaTime();
		m_AliveTime += dt;
		if (m_AliveTime > m_MaxLifeTime && !m_TrappedEnemy)
			Burst();
	}

	void BubbleScript::FixedUpdate()
	{
		float dt = Timer::GetInstance().GetFixedTimeStep();
		glm::vec2 pos = GetGameObject().GetWorldPosition();

		float absVelX = std::abs(m_Vel.x);
		if (absVelX > 0.1f)
		{
			float deceleration = m_DecX * dt;

			if (m_Vel.x > 0.f)
				m_Vel.x = std::max(0.f, m_Vel.x - deceleration);
			else if (m_Vel.x < 0.f)
				m_Vel.x = std::min(0.f, m_Vel.x + deceleration);

			pos.x += m_Vel.x * dt;
		}
		else
			m_Vel.x = 0.f;

		pos.y += m_Vel.y * dt;

		GetGameObject().SetWorldPosition(pos.x, pos.y);
	}

	void BubbleScript::OnNotify(EventInfo& event)
	{
		switch (event.GetID())
		{
		case JRE::Events::Box2DCollisionEvent::ID:
		{
			auto& args = event.GetArgs<JRE::Events::Box2DCollisionEvent>();
			if (args.other.GetProperties().layer & CollisionLayer::Enemy)
			{
				auto& enemy = args.other.GetOwner();
				auto pScript = enemy.GetComponent<ZenchanScriptComponent>();
					Trap(pScript);
			}
			else if (args.other.GetProperties().layer & CollisionLayer::Friendly && m_AliveTime > 0.5f)
				PopAndKill();
			break;
		}
		case JRE::Events::EndOfClipEvent::ID:
		{
			auto& args = event.GetArgs<JRE::Events::EndOfClipEvent>();
			if (args.clip == m_pPoofAnimClip)
				GetGameObject().Destroy();
			break;
		}
		}
	}

	void BubbleScript::Trap(ZenchanScriptComponent* enemy)
	{
		m_TrappedEnemy = enemy;
		//enemy->SetTrapped(true);
		// Possibly change bubble animation
	}

	void BubbleScript::PopAndKill()
	{
		m_pSpriteAnimCmp->SetActiveClip("Poof");
		if (m_TrappedEnemy)
		{
			//m_TrappedEnemy->Kill();
			SpawnFood();
		}
	}

	void BubbleScript::Burst()
	{
		m_pSpriteAnimCmp->SetActiveClip("Poof");
		//if (m_TrappedEnemy)
		//	m_TrappedEnemy->SetTrapped(false);
	}

	void BubbleScript::SpawnFood()
	{
		//auto food = std::make_unique<GameObject>("Food");
		//food->SetLocalPosition(GetOwner().GetLocalTransform().position);
		//food->AddComponent<SpriteRenderer>("food_texture");
		//// You might want to add a PickupComponent or similar here

		//GetOwner().GetScene()->Add(std::move(food));
	}
}

