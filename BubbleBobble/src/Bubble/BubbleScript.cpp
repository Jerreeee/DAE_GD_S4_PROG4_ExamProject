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

		m_Box2DCollisionEventConn = m_pBox2ColliderCmp->OnCollisionEvent.AddObserver(
			[this](JRE::EventInfo& e) { OnCollision(e); });
		m_PoofAnimClip = m_pSpriteAnimCmp->GetClip("Poof");
		m_PoofAnimClipEndOfClipEventConn = m_PoofAnimClip->OnEndOfClipEvent.AddObserver(
			[this](JRE::EventInfo&) { GetGameObject().Destroy(); });
	}

	void BubbleScript::Update()
	{
		if (m_Popped) return;

		float dt = Timer::GetInstance().GetDeltaTime();
		m_AliveTime += dt;
		if (m_AliveTime > m_MaxLifeTime)
			Burst();
	}

	void BubbleScript::FixedUpdate()
	{
		if (m_Popped) return;

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

	void BubbleScript::OnCollision(EventInfo& event)
	{
		auto& args = event.GetArgs<JRE::Events::Box2DCollisionEvent>();
		if (args.other.GetProperties().layer & CollisionLayer::Enemy && !m_TrappedEnemy)
		{
			auto& enemy = args.other.GetOwner();
			auto pScript = enemy.GetComponent<ZenchanScriptComponent>();
			if (!pScript || pScript->IsTrapped())
				return;

			m_TrappedEnemy = pScript;
			pScript->SetTrappedBy(&GetGameObject());
		}
		else if (args.other.GetProperties().layer & CollisionLayer::Friendly && m_AliveTime > 0.5f)
			PopAndKill();
	}

	void BubbleScript::PopAndKill()
	{
		if (m_TrappedEnemy)
		{
			m_TrappedEnemy->Kill();
			m_TrappedEnemy = nullptr;
		}
		Burst();
	}

	void BubbleScript::Burst()
	{
		m_pSpriteAnimCmp->SetActiveClip("Poof");
		m_pBox2ColliderCmp->SetEnabled(false);
		if (m_TrappedEnemy)
		{
			m_TrappedEnemy->SetTrappedBy(nullptr);
			m_TrappedEnemy = nullptr;
		}
		m_Popped = true;
	}
}

