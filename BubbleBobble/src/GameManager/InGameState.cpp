#include <filesystem>
#include "JREngine/Scene/SceneManager.h"

#include "Player/PlayerScriptComponent.h"
#include "SceneBuilders/LevelBuilder.h"
#include "GameManagerComponent.h"
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
		auto pPlayerGameObject = scene.GetGameObjectByComponentType<Player::ScriptComponent>();
		pPlayerGameObject;
		// 2) Load player spawn pos from file
		// 3) Set player pos to spawn pos in file

		// 4) Load enemy spawn positions form file
		// 5) spawn enemies at positions
	}
}
