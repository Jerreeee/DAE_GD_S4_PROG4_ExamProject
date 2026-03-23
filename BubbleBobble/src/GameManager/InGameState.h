#pragma once
#include "JREngine/Core/Event.h"
#include "GameManager/IGameState.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"

namespace JRE
{
	class TextRendererComponent;
}

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
		void HandlePlayerLostLive(JRE::EventInfo& e);
		void HandleEnemyDied(JRE::EventInfo& e);
		void SkipLevel();
		void GoToNextLevel(JRE::SceneManager::OnSceneLoadCallBack loadCallback = {});
		void SetPlayerToSpawnPos(JRE::Scene& scene);
		void CreatePlayerAndUI(JRE::Scene& scene);
		void CreateEnemies(JRE::Scene& scene);

		GameManagerComponent* m_pGameManagerComponent{ nullptr };
		
		int m_Score{};
		int m_NrLevels{ 3 };
		int m_LevelIdx{ 0 };
		bool m_PlayerDied{};
		int m_NrEnemiesToKill{};
		int m_NrEnemiesKilled{};
		size_t m_P1ActionMapIdx{};

		JRE::TextRendererComponent* m_pScoreTxtCmp{};
		std::shared_ptr<JRE::EventConnection> m_PlayerLostLifeEventConn{ nullptr };
		std::vector<std::shared_ptr<JRE::EventConnection>> m_EnemyDiedConns{};
	};
}
