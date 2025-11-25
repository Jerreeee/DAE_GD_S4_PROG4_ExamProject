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
		//Resources
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

		// Create state instances
		m_States.emplace_back(std::make_unique<PlayerState_Mortal>(*this));
		m_States.emplace_back(std::make_unique<PlayerState_Immortal>(*this, 4.0f));
		m_States.emplace_back(std::make_unique<PlayerState_Dead>(*this));
	}

	PlayerScriptComponent::~PlayerScriptComponent()
	{
		if (m_ShootClipEndOfClipEventConn)
			m_ShootClipEndOfClipEventConn->Disconnect(this);

		if (m_DeathClipEndOfClipEventConn)
			m_DeathClipEndOfClipEventConn->Disconnect(this);
	}

	void PlayerScriptComponent::Start()
	{
		m_States[static_cast<size_t>(m_State)]->OnEnter();
	}

	void PlayerScriptComponent::Update()
	{
		m_States[static_cast<size_t>(m_State)]->Update();

		if (m_HasPendingStateChange)
			ApplyStateChange();
	}

	void PlayerScriptComponent::FixedUpdate()
	{
		m_States[static_cast<size_t>(m_State)]->FixedUpdate();
	}
	void PlayerScriptComponent::OnNotify(JRE::EventInfo& event)
	{
		m_States[static_cast<size_t>(m_State)]->OnEvent(event);
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
	void PlayerScriptComponent::ApplyStateChange()
	{
		m_HasPendingStateChange = false;

		if (m_State == m_NextState)
			return;

		m_States[(size_t)m_State]->OnExit();
		m_State = m_NextState;
		m_States[(size_t)m_State]->OnEnter();
	}
	void PlayerScriptComponent::RequestStateChange(PlayerState newState)
	{
		m_NextState = newState;
		m_HasPendingStateChange = true;
	}
	void PlayerScriptComponent::MoveUpdate()
	{
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
