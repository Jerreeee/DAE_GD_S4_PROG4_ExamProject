#include <iostream>
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Physics/Box2DColliderComponent.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Core/ServiceLocator.h"

#include "Utils.h"
#include "EngineSetup.h"
#include "Enemies/Zenchan/ZenchanScriptComponent.h"

using namespace JRE;

namespace BubbleBobble
{
	ZenchanScriptComponent::ZenchanScriptComponent(JRE::GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		m_pSpriteRendererCmp = GetGameObject().GetComponent<SpriteRendererComponent>();
		assert(m_pSpriteRendererCmp && "Player doesnt have SpriteRendererComponent");
		m_pSpriteAnimatorCmp = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		assert(m_pSpriteAnimatorCmp && "Player doesnt have SpriteAnimatorComponent");
		m_pBox2DColliderCmp = GetGameObject().GetComponent<Box2DColliderComponent>();
		assert(m_pBox2DColliderCmp && "Player doesnt have Box2DColliderComponent");
	}

	void ZenchanScriptComponent::Update()
	{
		switch (m_State)
		{
		case State::Normal:
		{
			//Flip sprites based on movement direction
			if (m_Input.moveDir != 0)
				m_FacingDir = m_Input.moveDir;
			m_pSpriteRendererCmp->SetFlipX(m_FacingDir == -1);

			bool onGround = m_CollInfo.collDir.down;
			if (onGround && m_Input.pressedJump)
				m_Vel.y = -m_JumpForce;

			m_Input = Input{}; //Consume all input
			break;
		}
		case State::Trapped:
		{
			auto pos = glm::vec2(m_pBubble->GetWorldPosition());
			GetGameObject().SetWorldPosition(pos.x, pos.y);
			break;
		}
		}
	}

	void ZenchanScriptComponent::FixedUpdate()
	{
		glm::vec3 oldPos = GetGameObject().GetWorldPosition();

		BoxPhysicsSystem::CollisionSettings cs{ oldPos, *m_pBox2DColliderCmp };
		cs.dt = Timer::GetInstance().GetFixedTimeStep();
		cs.applyGravity = true;
		cs.filterFunc = Utils::PlatformCollisionDirFilterFunc;

		//Calc vel
		m_Vel.x = m_Speed * m_Input.moveDir;
		cs.vel = m_Vel;

		//Check collision
		BoxPhysicsSystem& physicsSystem = static_cast<BoxPhysicsSystem&>(ServiceLocator::GetPhysicsSystem());
		physicsSystem.MoveCollider(cs, m_CollInfo);
		GetGameObject().SetWorldPosition(m_CollInfo.newPos.x, m_CollInfo.newPos.y);
		m_Vel = m_CollInfo.velOut;
	}

	void ZenchanScriptComponent::Move(int direction)
	{
		m_Input.moveDir = std::clamp(direction, -1, 1);
	}

	void ZenchanScriptComponent::Jump()
	{
		if (!m_Input.pressedJump)
			m_Input.pressedJump = true;
	}
	void ZenchanScriptComponent::Kill()
	{
		if (GetGameObject().IsDestroyed()) return;
		EventInfo e = CreateEvent<Events::EnemyDied>(100);
		GetGameObject().Destroy();
		OnEnemyDied.Notify(e);
	}

	void ZenchanScriptComponent::SetTrappedBy(JRE::GameObject* bubble)
	{
		if (m_State == State::Normal && bubble)
		{
			m_pBubble = bubble;
			m_State = State::Trapped;
		}
		else if (m_State == State::Trapped && !bubble)
		{
			m_State = State::Normal;
		}

		//if (trapped)
		//{
		//	// Stop movement and AI
		//	// (optional: disable physics, or set to kinematic)
		//	// Optionally change animation to trapped state
		//	//auto* animator = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		//	//if (animator) animator->Play("Trapped");
		//}
		//else
		//{
		//	// Restore normal behavior
		//	//auto* animator = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		//	//if (animator) animator->Play("Walk");
		//}
	}
}
