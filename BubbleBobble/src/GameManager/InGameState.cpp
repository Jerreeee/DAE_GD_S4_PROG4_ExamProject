#include <filesystem>
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Input/InputManager.h"

#include "Player/PlayerScriptComponent.h"
#include "SceneBuilders/LevelBuilder.h"
#include "GameManagerComponent.h"
#include "Assets/LevelDataComponent.h"
#include "Player/PlayerBuilder.h"
#include "Enemies/Zenchan/ZenchanBuilder.h"
#include "TileMap/TileMapComponent.h"
#include "InGameState.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	InGameState::InGameState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "InGame")
	{
	}
	void InGameState::OnEnter()
	{
		auto& sm = SceneManager::GetInstance();
		auto& im = InputManager::GetInstance();

		//GameMode mode = m_GameManagerComponent.GetGameMode();

		//Create actionMap for the player and set bindings
		size_t actionMapIdx = im.AddActionMap({ 0 });
		im.BindCommand(actionMapIdx, "MoveLeft", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::A, KeyState::Pressed));
		im.BindCommand(actionMapIdx, "MoveRight", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::D, KeyState::Pressed));
		im.BindCommand(actionMapIdx, "Jump", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::W, KeyState::DownThisFrame));
		im.BindCommand(actionMapIdx, "Shoot", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::F, KeyState::DownThisFrame));

		//Load 1st level
		GoToNextLevel();

		//Create player(s) for gameMode
		auto pPlayer = std::make_unique<GameObject>("Player");
		PlayerBuilder()
			.SetAnimationPath("Anims/P1.txt")
			.SetActionMapIdx(actionMapIdx)
			.Build(pPlayer);
		sm.GetCurrentScene().Add(std::move(pPlayer));
		SetPlayerToSpawnPos();

		CreateEnemies();
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
	}
	void InGameState::SetPlayerToSpawnPos()
	{
		auto& sm = SceneManager::GetInstance();
		Scene& scene = sm.GetCurrentScene();

		//1) Find player in gameObjects (with tag, or PlayerScriptComponent)
		auto pPlayer = scene.GetGameObjectByComponentType<PlayerScriptComponent>();
		assert(pPlayer && "Couldn't find the player gameObject in the scene");
		//2) Find LevelData
		auto pLevelDataCmp = scene.GetComponent<LevelDataComponent>();
		//3) Set player spawnPos
		assert(pLevelDataCmp->m_LevelData->players.size() > 0 && "No player pos defined in the levelData");
		glm::vec2 spawnPos = pLevelDataCmp->m_LevelData->players[0];
		pPlayer->SetWorldPosition(spawnPos.x, spawnPos.y);
	}
	void InGameState::CreateEnemies()
	{
		auto& sm = SceneManager::GetInstance();
		Scene& scene = sm.GetCurrentScene();

		//1) Find LevelData
		auto pLevelDataCmp = scene.GetComponent<LevelDataComponent>();
		const auto& enemies = pLevelDataCmp->m_LevelData->enemies;

		//2) Create and add all enemies to the scene and set their spawnPos
		for (const auto& enemy : enemies)
		{
			switch (enemy.type)
			{
			case EnemyType::Zenchan:
			{
				auto pZenchan = std::make_unique<GameObject>("Zenchan");
				ZenchanBuilder()
					.Build(pZenchan);
				pZenchan->SetWorldPosition(enemy.pos.x, enemy.pos.y);
				scene.Add(std::move(pZenchan));
				break;
			}
			}
		}
	}
}
