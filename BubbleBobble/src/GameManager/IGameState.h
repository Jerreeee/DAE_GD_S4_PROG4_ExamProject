#pragma once
#include <string>

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
		IGameState(GameManagerComponent& gameManagerComponent, const std::string& name)
			: m_GameManagerComponent{ gameManagerComponent }, m_Name{ name }
		{}
		virtual ~IGameState() = default;
		virtual void OnEnter() {};
		virtual GameState Update() = 0;
		virtual void OnExit() {};

		const std::string GetName() const { return m_Name; };
	protected:
		GameManagerComponent& m_GameManagerComponent;
		std::string m_Name{};
	};
}
