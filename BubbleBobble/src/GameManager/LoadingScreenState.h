#pragma once
#include "IGameState.h"

namespace BubbleBobble
{
	class GameManagerComponent;
	class LoadingScreenState final : public IGameState
	{
	public:
		LoadingScreenState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;
	private:
	};
}
