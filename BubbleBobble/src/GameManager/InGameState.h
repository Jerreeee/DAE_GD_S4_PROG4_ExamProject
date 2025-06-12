#pragma once
#include "JREngine/Core/Event.h"
#include "GameManager/IGameState.h"

namespace BubbleBobble
{
	class InGameState final : public IGameState, public JRE::IObserver
	{
	public:
		InGameState(GameManagerComponent& gameManagerComponent);

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;

		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		void GoToNextLevel();
		void SetPlayerToSpawnPos();
		void CreateEnemies();
		
		int m_NrLevels{ 3 };
		int m_LevelIdx{ 0 };
	};
}
