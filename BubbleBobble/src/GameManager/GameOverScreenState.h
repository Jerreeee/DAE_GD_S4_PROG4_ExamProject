#pragma once
#include "JREngine/Core/Command.h"
#include "GameManager/IGameState.h"

namespace BubbleBobble
{
	class GameOverScreenState final : public IGameState
	{
	public:
		GameOverScreenState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
	private:
		class ReturnToMainMenuCommand final : public JRE::Command
		{
		public:
			ReturnToMainMenuCommand(GameOverScreenState& state) : m_State{ state } {}
			virtual void Execute() override { m_State.m_ReturnToMainMenu = true; }
		private:
			GameOverScreenState& m_State;
		};

		bool m_ReturnToMainMenu{};
	};
}
