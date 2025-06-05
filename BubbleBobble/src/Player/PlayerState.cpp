#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Input/InputManager.h"

#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerState.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble::Player
{
	void MovingState::OnEnter()
	{
		m_Player.ChangeAnimation(Animation::Run);
	}
	State MovingState::Update()
	{
		auto& im = InputManager::GetInstance();

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
		else if (jump)
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
