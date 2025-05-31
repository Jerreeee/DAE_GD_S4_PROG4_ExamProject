#pragma once
#include "JREngine/Core/Singleton.h"
#include "JREngine/Core/Observer.h"

namespace BubbleBobble
{
	enum class GameState
	{
		MainMenu, LoadingScreen, InGame, GameOverScreen
	};

	class GameInstance final : public JRE::Singleton<GameInstance>, public JRE::IObserver
	{
	public:
		void Init();

		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		friend class Singleton<GameInstance>;
		GameInstance() = default;

		GameState m_GameState{ GameState::MainMenu };
	};
}
