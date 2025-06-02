#pragma once

namespace BubbleBobble
{
	enum class GameState
	{
		MainMenu, LoadingScreen, InGame, GameOverScreen, None
	};

	class GameManagerComponent;
	class IGameState
	{
	public:
		IGameState(GameManagerComponent& gameManagerComponent) : m_GameManagerComponent{ gameManagerComponent } {}
		virtual ~IGameState() = default;
		virtual void OnEnter() {};
		virtual GameState Update() = 0;
		virtual void OnExit() {};
	protected:
		GameManagerComponent& m_GameManagerComponent;
	};
}
