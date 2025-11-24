#pragma once
#include "JREngine/Core/Command.h"
#include "GameManager/IGameState.h"

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
		void Reset();

		bool m_StartSinglePlayer{ false };
		size_t m_ActionMapIdx{};
	};
}
