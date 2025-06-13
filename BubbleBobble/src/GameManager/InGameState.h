#pragma once
#include "JREngine/Core/Event.h"
#include "GameManager/IGameState.h"
#include "JREngine/Scene/SceneManager.h"

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
		void GoToNextLevel(JRE::SceneManager::OnSceneLoadCallBack loadCallback = {});
		void SetPlayerToSpawnPos();
		void CreateEnemies();
		
		int m_NrLevels{ 3 };
		int m_LevelIdx{ 0 };
		bool m_PlayerDied{};
		
		JRE::SceneManager::OnSceneLoadCallBack m_LoadCallback{};
	};
}
