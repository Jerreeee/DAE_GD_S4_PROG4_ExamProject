#pragma once
#include "JREngine/Core/Singleton.h"
#include "JREngine/Core/Observer.h"
#include "JREngine/Core/Command.h"

#include "GameStates.h"

namespace BubbleBobble
{
	class GameInstance final : public JRE::Singleton<GameInstance>, public JRE::IObserver
	{
	public:
		void Init(GameState state = GameState::MainMenu);

		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		friend class Singleton<GameInstance>;
		GameInstance() = default;

		//MainMenuState m_MainMenuState;
		GameState m_GameState{ GameState::MainMenu };
		std::vector<std::string> m_SceneNames{ "MainMenu", "LoadingScreen", "InGame", "GameOverScreen" };
	};
}
