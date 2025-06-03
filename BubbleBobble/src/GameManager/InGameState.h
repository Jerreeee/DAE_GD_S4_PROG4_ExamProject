#pragma once
#include "GameManager/IGameState.h"

namespace BubbleBobble
{
	class InGameState final : public IGameState
	{
	public:
		InGameState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;
	private:
		int m_NrLevels{ 3 };
		int m_LevelIdx{ 1 };
	};
}
