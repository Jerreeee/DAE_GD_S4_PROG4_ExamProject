#include "JREngine/Animation/SpriteAnimationClip.h"
#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerState.h"

namespace BubbleBobble::Player
{
	void MovingState::OnEnter()
	{
		m_Player.ChangeAnimation(Animation::Run);
	}
	State MovingState::Update()
	{
		const Input& input = m_Player.m_Input;

		m_Player.Move(input.moveDir);

		if (input.moveDir == 0)
			m_Player.ChangeAnimation(Animation::Idle);
		else
			m_Player.ChangeAnimation(Animation::Run);
		//TODO Flip running animation depending on going left or right

		if (input.pressedShoot)
			return State::Shooting;
		else if (input.pressedJump) //TODO only if on the ground
			return State::Jumping;
		return State::None;
	}

	void ShootState::OnEnter()
	{
		m_AnimClipEnded = false;
		m_Player.ChangeAnimation(Animation::Shoot);
	}
	State ShootState::Update()
	{
		const Input& input = m_Player.m_Input;

		m_Player.Move(input.moveDir);

		if (!m_AnimClip || m_AnimClipEnded)
			return State::Moving;
		else if (input.pressedJump)
			return State::Jumping;
		return State::None;
	}
	void ShootState::SetAnimationClip(JRE::AssetRef<JRE::SpriteAnimationClip> animClip)
	{
		if (m_AnimClip)
			m_AnimClip->OnEndOfClipEvent.RemoveObserver(this);

		m_AnimClip = animClip;
		if (m_AnimClip)
			m_AnimClip->OnEndOfClipEvent.AddObserver(this);
	}
	void ShootState::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case JRE::Events::EndOfClipEvent::ID:
			m_AnimClipEnded = true;
			break;
		}
	}


	void DiedState::OnEnter()
	{
		m_Player.ChangeAnimation(Animation::Death);
	}


	void JumpState::OnEnter()
	{
		m_Player.Jump();
	}
	State JumpState::Update()
	{
		return State::Moving; //immediatly return to move state
	}
}
