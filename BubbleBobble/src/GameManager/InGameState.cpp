#include <filesystem>
#include "JREngine/Input/InputManager.h"

#include "EngineSetup.h"
#include "SceneBuilders/UIBuilder.h"
#include "Player/PlayerScriptComponent.h"
#include "SceneBuilders/LevelBuilder.h"
#include "GameManagerComponent.h"
#include "Assets/LevelDataComponent.h"
#include "Player/PlayerBuilder.h"
#include "Enemies/Zenchan/ZenchanBuilder.h"
#include "TileMap/TileMapComponent.h"
#include "GameManager/InGameState.h"

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
		//GameMode mode = m_GameManagerComponent.GetGameMode();

		//Callback exectued only when the 1st level is loaded
		//This creates the player,UI gameObjects's
		SceneManager::OnSceneLoadCallBack loadCallback = [&](Scene& scene) -> void {
			CreatePlayerAndUI(scene);
			SetPlayerToSpawnPos(scene);
			CreateEnemies(scene);
			};

		//Load 1st level
		GoToNextLevel(loadCallback);
	}
	GameState InGameState::Update()
	{
		if (m_PlayerDied)
			return GameState::GameOverScreen;
		return GameState::None;
	}
	void InGameState::OnExit()
	{
	}
	void InGameState::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case Events::PlayerLostLive::ID:
		{
			auto& args = event.GetArgs<Events::PlayerLostLive>();
			if (args.health <= 0)
				m_PlayerDied = true;
			else
			{
				SceneManager::OnSceneLoadCallBack loadCallback = [&](Scene& scene) -> void {
					SetPlayerToSpawnPos(scene);
					CreateEnemies(scene);
					};
				GoToNextLevel(loadCallback);
				//SetPlayerToSpawnPos();
			}
			break;
		}
		}
	}
	void InGameState::GoToNextLevel(JRE::SceneManager::OnSceneLoadCallBack loadCallback)
	{
		++m_LevelIdx;
		if (m_LevelIdx > m_NrLevels)
			return;

		auto& sm = SceneManager::GetInstance();

		//1) Load level
		std::string sLevelIdx = std::to_string(m_LevelIdx);
		std::string path{"Data/Levels/" + sLevelIdx };
		std::string levelName{ "Level_" + sLevelIdx };

		if (!sm.HasScene(levelName))
		{
			auto& levelScene = sm.CreateScene(levelName, PersistenceMask::LevelScene);
			LevelBuilder(levelScene, path).Build();
		}

		sm.SetNextScene(levelName, loadCallback);
	}
	void InGameState::SetPlayerToSpawnPos(Scene& scene)
	{
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
	void InGameState::CreatePlayerAndUI(Scene& scene)
	{
		auto& im = InputManager::GetInstance();

		//Create actionMap for the player and set bindings
		size_t actionMapIdx = im.AddActionMap({ 0 });
		im.BindCommand(actionMapIdx, "MoveLeft", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::A, KeyState::Pressed));
		im.BindCommand(actionMapIdx, "MoveRight", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::D, KeyState::Pressed));
		im.BindCommand(actionMapIdx, "Jump", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::W, KeyState::DownThisFrame));
		im.BindCommand(actionMapIdx, "Shoot", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::F, KeyState::DownThisFrame));

		//Create player(s) for gameMode
		auto pPlayer = std::make_unique<GameObject>("Player");
		PlayerBuilder()
			.SetAnimationPath("Anims/P1.txt")
			.SetActionMapIdx(actionMapIdx)
			.Build(pPlayer);

		auto playerScriptCmp = pPlayer->GetComponent<PlayerScriptComponent>();
		playerScriptCmp->OnPlayerLostLive.AddObserver(this);

		UIBuilder(scene)
			.SetPlayer1(*pPlayer)
			.Build();

		scene.Add(std::move(pPlayer));
	}
	void InGameState::CreateEnemies(Scene& scene)
	{
		//1) Find LevelData
		auto pLevelDataCmp = scene.GetComponent<LevelDataComponent>();
		const auto& enemies = pLevelDataCmp->m_LevelData->enemies;

		//2) Find player to set as the enemy target
		auto pPlayer = scene.GetGameObjectByComponentType<PlayerScriptComponent>();
		assert(pPlayer && "Couldn't find the player gameObject in the scene");

		//3) Create and add all enemies to the scene and set their spawnPos
		for (const auto& enemy : enemies)
		{
			switch (enemy.type)
			{
			case EnemyType::Zenchan:
			{
				auto pZenchan = std::make_unique<GameObject>("Zenchan");
				ZenchanBuilder()
					.SetupAIController(pPlayer)
					.Build(pZenchan);
				pZenchan->SetWorldPosition(enemy.pos.x, enemy.pos.y);
				scene.Add(std::move(pZenchan));
				break;
			}
			}
		}
	}
}
