#include <filesystem>
#include "JREngine/Scene/SceneManager.h"

#include "Player/PlayerScriptComponent.h"
#include "SceneBuilders/LevelBuilder.h"
#include "GameManagerComponent.h"
#include "Assets/LevelDataComponent.h"
#include "InGameState.h"

using namespace JRE;

namespace BubbleBobble
{
	InGameState::InGameState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "InGame")
	{
	}
	void InGameState::OnEnter()
	{
		GoToNextLevel();
	}
	GameState InGameState::Update()
	{
		return GameState::None;
	}
	void InGameState::OnExit()
	{
	}
	void InGameState::GoToNextLevel()
	{
		++m_LevelIdx;
		if (m_LevelIdx > m_NrLevels)
			return;

		auto& sm = SceneManager::GetInstance();

		//1) Load level
		std::string sLevelIdx = std::to_string(m_LevelIdx);
		std::string path{"Data/Levels/" + sLevelIdx };
		std::string levelName{ "Level_" + sLevelIdx };
		auto& levelScene = sm.CreateScene(levelName);
		LevelBuilder(levelScene, path).Build();

		sm.LoadScene(levelName);
		Scene& scene = sm.GetCurrentScene();

		// 1) Find player in gameObjects (with tag, or PlayerScriptComponent)
		auto pPlayer = scene.GetGameObjectByComponentType<Player::ScriptComponent>();
		assert(pPlayer && "Couldn't find the player gameObject in the scene");
		// 2) Find LevelData
		auto pLevelDataCmp = scene.GetComponent<LevelDataComponent>();
		// 3) Set player pos to spawn pos
		assert(pLevelDataCmp->m_LevelData->players.size() > 0 && "No player pos defined in the levelData");
		glm::vec2 spawnPos = pLevelDataCmp->m_LevelData->players[0];
		pPlayer->SetWorldPosition(spawnPos.x, spawnPos.y);

		// 4) Load enemy spawn positions form file
		// 5) spawn enemies at positions
	}
}
