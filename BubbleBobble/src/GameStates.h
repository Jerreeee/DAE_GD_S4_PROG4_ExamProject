#pragma once
#include "JREngine/Core/Command.h"

namespace BubbleBobble
{
	enum class GameState
	{
		MainMenu, LoadingScreen, InGame, GameOverScreen
	};

	class SetGameStateCommand final : public JRE::Command
	{
	public:
		SetGameStateCommand(GameState& gameState, GameState newState)
			: m_GameState{ gameState }
			, m_NewState{ newState }
		{
		}

		virtual void Execute() override
		{
			m_GameState = m_NewState;
		}
	private:
		GameState& m_GameState;
		GameState m_NewState;
	};

	class IGameState
	{
	public:
		virtual ~IGameState() = default;
		virtual void OnEnter() {};
		virtual GameState Update() = 0;
		virtual void OnExit() {};
	};

	//class MainMenuState final : public IGameState
	//{
	//public:
	//private:
	//	class StartSinglePlayerCommand final : public JRE::Command
	//	{
	//	public:
	//	}

	//	//bool m_StartSinglePlayer
	//};
}
