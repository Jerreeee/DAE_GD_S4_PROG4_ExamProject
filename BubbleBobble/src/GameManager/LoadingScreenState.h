#pragma once
#include "IGameState.h"

namespace BubbleBobble
{
	class LoadingScreenState final : public IGameState
	{
	public:
		LoadingScreenState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;
	private:
		float m_MaxTime{ 2.f };
		float m_Timer{};
	};
}
