#include <iostream>
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Audio/ISoundSystem.h"

#include "Bubble/BubbleBuilder.h"
#include "Utils.h"
#include "EngineSetup.h"
#include "Player/PlayerScriptComponent.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	PlayerScriptComponent::PlayerScriptComponent(JRE::GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		m_pSpriteRendererCmp = GetGameObject().GetComponent<SpriteRendererComponent>();
		assert(m_pSpriteRendererCmp && "Player doesnt have SpriteRendererComponent");
		m_pSpriteAnimatorCmp = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		assert(m_pSpriteAnimatorCmp && "Player doesnt have SpriteAnimatorComponent");
		m_pBox2DColliderCmp = GetGameObject().GetComponent<Box2DColliderComponent>();
		assert(m_pBox2DColliderCmp && "Player doesnt have Box2DColliderComponent");
		m_pHealthCmp = GetGameObject().GetComponent<HealthComponent>();
		assert(m_pHealthCmp && "Player doesnt have HealthComponent");

		m_pBox2DColliderCmp->OnCollisionEvent.AddObserver(this);

		m_pShootClipRef = m_pSpriteAnimatorCmp->GetClip("Shoot");
		m_ShootClipEndOfClipEventConn = m_pShootClipRef->OnEndOfClipEvent.AddObserver(this);

		m_pDeathClipRef = m_pSpriteAnimatorCmp->GetClip("Death");
		m_DeathClipEndOfClipEventConn = m_pDeathClipRef->OnEndOfClipEvent.AddObserver(this);
	}

	PlayerScriptComponent::~PlayerScriptComponent()
	{
		if (m_ShootClipEndOfClipEventConn)
			m_ShootClipEndOfClipEventConn->Disconnect(this);

		if (m_DeathClipEndOfClipEventConn)
			m_DeathClipEndOfClipEventConn->Disconnect(this);
	}

	void PlayerScriptComponent::Update()
	{
		float dt = Timer::GetInstance().GetDeltaTime();

		switch (m_State)
		{
		case State::Immortal:
			m_ImmortalTimer -= dt;
			if (m_ImmortalTimer < 0.f)
				m_State = State::Mortal;
		case State::Mortal:
			if (m_Input.moveDir != 0)
				m_FacingDir = m_Input.moveDir;

			if (m_AnimState == AnimState::Moving)
			{
				if (m_Input.moveDir)
					m_pSpriteAnimatorCmp->SetActiveClip("Run");
				else
					m_pSpriteAnimatorCmp->SetActiveClip("Idle");

			}
			//Flip sprites based on movement direction
			m_pSpriteRendererCmp->SetFlipX(m_FacingDir == -1);

			if (m_Input.pressedShoot)
				ShootBubble(); //TODO add shoot animation

			bool onGround = m_CollInfo.collDir.down;
			if (onGround && m_Input.pressedJump)
			{
				m_Vel.y = -m_JumpForce;
				if (m_JumpSound)
					ServiceLocator::GetSoundSystem().Play(m_JumpSound.Get());
			}

			m_Input = Input{}; //Consume all input
			break;

		}
	}

	void PlayerScriptComponent::FixedUpdate()
	{
		switch (m_State)
		{
		case State::Mortal:
		case State::Immortal:
			MoveCollider();
			break;
		}
	}
	void PlayerScriptComponent::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case JRE::Events::EventDestroyed::ID:
		{
			//auto& args = event.GetArgs<JRE::Events::EventDestroyed>();
			//args.event.RemoveObserver(this);
			break;
		}
		case JRE::Events::Box2DCollisionEvent::ID:
		{
			if (m_State != State::Mortal)
				break;
			auto& args = event.GetArgs<JRE::Events::Box2DCollisionEvent>();
			if (args.other.GetProperties().layer & CollisionLayer::Enemy)
			{
				m_State = State::Death;
				m_AnimState = AnimState::Death;
				m_pSpriteAnimatorCmp->SetActiveClip("Death");
				m_pSpriteRendererCmp->SetOffset(glm::vec2(0.f, -45.f));
			}
			break;
		}
		case JRE::Events::EndOfClipEvent::ID:
		{
			auto& args = event.GetArgs<JRE::Events::EndOfClipEvent>();
			if (args.clip == m_pDeathClipRef.get() && m_State == State::Death)
			{
				m_pHealthCmp->TakeDamage(1);
				EventInfo e = CreateEvent<Events::PlayerLostLive>(m_pHealthCmp->GetHealth());
				OnPlayerLostLive.Notify(e);
				m_State = State::Immortal;
				m_AnimState = AnimState::Moving;
				m_ImmortalTimer = m_ImmortalTimerDefault;
				m_pSpriteRendererCmp->SetOffset();
			}
			else if (args.clip == m_pShootClipRef.get())
				m_AnimState = AnimState::Moving;
			break;
		}
		}
	}
	void PlayerScriptComponent::Move(int direction)
	{
		m_Input.moveDir = std::clamp(direction, -1, 1);
	}
	void PlayerScriptComponent::Jump()
	{
		if (!m_Input.pressedJump)
			m_Input.pressedJump = true;
	}
	void PlayerScriptComponent::Shoot()
	{
		if (!m_Input.pressedShoot)
			m_Input.pressedShoot = true;
	}
	void PlayerScriptComponent::MoveCollider()
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
	void PlayerScriptComponent::ShootBubble()
	{
		glm::vec2 spawnPos = glm::vec2(GetGameObject().GetWorldPosition()) + glm::vec2(m_FacingDir == 1 ? 10.0f : -10.0f, 0.0f);
		auto bubble = BubbleBuilder().Build(GetGameObject().GetName(), spawnPos, m_FacingDir);
		SceneManager::GetInstance().GetCurrentScene().Add(std::move(bubble));
		if (m_ShootSound)
			ServiceLocator::GetSoundSystem().Play(m_ShootSound.Get());
		m_pSpriteAnimatorCmp->SetActiveClip("Shoot");
		m_AnimState = AnimState::Shoot;
	}
}
