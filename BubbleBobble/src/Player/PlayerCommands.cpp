#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerCommands.h"

namespace BubbleBobble::Player
{
	void MoveCommand::Execute()
	{
		m_Player.m_Input.moveDir = m_Dir;
	}

	void JumpCommand::Execute()
	{
		m_Player.m_Input.pressedJump = true;
	}

	void BubbleBobble::Player::ShootCommand::Execute()
	{
		m_Player.m_Input.pressedShoot = true;
	}
}
