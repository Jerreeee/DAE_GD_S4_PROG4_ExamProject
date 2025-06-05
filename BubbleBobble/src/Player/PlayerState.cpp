#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Physics/RigidBody2DComponent.h"

#include "TileMap/TileMapPhysicsSystem.h"
#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerState.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble::Player
{
	MovingState::MovingState(ScriptComponent& player, const JRE::Input::ActionMap*& pActionMap, float speed)
		: m_Player{ player }
		, m_pActionMap{ pActionMap }
		, m_pRigidBody{ m_Player.GetGameObject().GetComponent<RigidBody2DComponent>() }
		, m_Speed{ speed }
		, m_pPhysicsSystem{ JRE::SceneManager::GetInstance().GetCurrentScene().GetSystem<TileMapPhysicsSystem>() }
	{
	}
	void MovingState::OnEnter()
	{
		m_Player.ChangeAnimation(Animation::Run);
	}
	State MovingState::Update()
	{
		auto& im = InputManager::GetInstance();

		//Ground check
		bool onGround = false;
		if (m_pRigidBody && m_pRigidBody->GetVelocity().y > 0.f)
		{
			glm::vec3 worldPos = m_Player.GetGameObject().GetWorldPosition();
			onGround = m_pPhysicsSystem->IsRectOverlappingCollider(Region{ worldPos.x + 1, worldPos.y + 47, 46.f, 2.f });
			if (onGround)
				m_pRigidBody->SetVelocity(glm::vec2{ 0.f, 0.f });
		}


		bool moveLeft = im.IsBindingActive(*m_pActionMap, "MoveLeft");
		bool moveRight = im.IsBindingActive(*m_pActionMap, "MoveRight");
		if (moveLeft)
			m_Player.Move(-1, m_Speed);
		if (moveRight)
			m_Player.Move(1, m_Speed);

		if (moveLeft || moveRight)
			m_Player.ChangeAnimation(Animation::Idle);
		else
			m_Player.ChangeAnimation(Animation::Run);
		//TODO Flip running animation depending on going left or right

		bool jump = im.IsBindingActive(*m_pActionMap, "Jump");
		bool shoot = im.IsBindingActive(*m_pActionMap, "Shoot");
		if (shoot)
			return State::Shooting;
		else if (onGround && jump)
			return State::Jumping;
		return State::None;
	}


	void DiedState::OnEnter()
	{
		m_Player.ChangeAnimation(Animation::Death);
	}


	void JumpState::OnEnter()
	{
		m_Player.Jump(m_Force);
	}
	void ShootState::OnEnter()
	{
		m_Player.ChangeAnimation(Animation::Shoot);
	}
}
