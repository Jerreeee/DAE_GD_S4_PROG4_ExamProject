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
	class InGameState final : public IGameState, public JRE::IObserver
	{
	public:
		InGameState(GameManagerComponent& gameManagerComponent);
		~InGameState();

		virtual void OnEnter() override;
		virtual GameState Update() override;
		virtual void OnExit() override;

		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		class SkipLevel final : public JRE::Command
		{
		public:
			SkipLevel(InGameState& state) : m_State{ state } {}
			virtual void Execute() override;
		private:
			InGameState& m_State;
		};

		void GoToNextLevel(JRE::SceneManager::OnSceneLoadCallBack loadCallback = {});
		void SetPlayerToSpawnPos(JRE::Scene& scene);
		void CreatePlayerAndUI(JRE::Scene& scene);
		void CreateEnemies(JRE::Scene& scene);
		
		int m_Score{};
		int m_NrLevels{ 3 };
		int m_LevelIdx{ 0 };
		bool m_PlayerDied{};
		int m_NrEnemiesToKill{};
		int m_NrEnemiesKilled{};
		size_t m_P1ActionMapIdx{};

		JRE::TextRendererComponent* m_pScoreTxtCmp{};
		std::shared_ptr<JRE::EventConnection> m_PlayerLostLifeEventConn{ nullptr };
	};
}
