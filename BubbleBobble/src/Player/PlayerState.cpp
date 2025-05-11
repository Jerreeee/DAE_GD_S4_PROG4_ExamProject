#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerState.h"

#include <iostream>

namespace BubbleBobble
{
	void PlayerIdleState::OnEnter()
	{
		std::cout << "Idle\n";
	}
	void PlayerIdleState::OnHandleInput(const PlayerInput& input)
	{
		m_Moving = std::abs(input.moveDir.x) > m_Eps;
	}
	std::unique_ptr<IPlayerState> PlayerIdleState::Update()
	{
		if (m_Moving)
			return std::make_unique<PlayerRunningState>(m_Player);
		return nullptr;
	}


	void PlayerRunningState::OnEnter()
	{
		std::cout << "Running\n";
	}

	void PlayerRunningState::OnHandleInput(const PlayerInput& input)
	{
		if (input.moveDir.x < -m_Eps)
			m_Player.Move(glm::vec2{ -1.f, 0.f });
		else if (input.moveDir.x > m_Eps)
			m_Player.Move(glm::vec2{ 1.f, 0.f });
		m_Moving = std::abs(input.moveDir.x) > m_Eps;
	}
	std::unique_ptr<IPlayerState> PlayerRunningState::Update()
	{
		if (!m_Moving)
			return std::make_unique<PlayerIdleState>(m_Player);
		return nullptr;
	}
}
