#pragma once
#include "JREngine/Core/Command.h"
#include "IGameState.h"

namespace BubbleBobble
{
	class MainMenuState final : public IGameState
	{
	public:
		MainMenuState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;
	private:
		class StartSinglePlayerCommand final : public JRE::Command
		{
		public:
			StartSinglePlayerCommand(MainMenuState& state) : m_State{ state } {}
			virtual void Execute() override { m_State.m_StartSinglePlayer = true; }
		private:
			MainMenuState& m_State;
		};

		void Reset();

		bool m_StartSinglePlayer{ false };
		size_t m_ActionMapIdx{};
	};
}
