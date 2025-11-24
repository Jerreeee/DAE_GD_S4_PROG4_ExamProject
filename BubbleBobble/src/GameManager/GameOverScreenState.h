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
		virtual void OnExit() override;
	private:
		bool m_ReturnToMainMenu{};

		size_t m_ActionMapIdx{};
	};
}
