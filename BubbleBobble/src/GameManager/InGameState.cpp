#include <filesystem>
#include <iostream>

#include "JREngine/Input/InputManager.h"
#include "JREngine/Rendering/TextRendererComponent.h"

#include "EngineSetup.h"
#include "SceneBuilders/UIBuilder.h"
#include "Player/PlayerScriptComponent.h"
#include "SceneBuilders/LevelBuilder.h"
#include "GameManagerComponent.h"
#include "Assets/LevelDataComponent.h"
#include "Player/PlayerBuilder.h"
#include "Enemies/Zenchan/ZenchanBuilder.h"
#include "TileMap/TileMapComponent.h"
#include "Enemies/Zenchan/ZenchanScriptComponent.h"
#include "GameManager/InGameState.h"
#include "GameManager/GameManagerComponent.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	InGameState::InGameState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "InGame")
		, m_pGameManagerComponent{ &gameManagerComponent }
	{
		auto& im = InputManager::GetInstance();

		//Create actionMap for the player and set bindings
		m_P1ActionMapIdx = im.AddActionMap({ 0, 1 });
		im.BindCommand(m_P1ActionMapIdx, "SkipLevel",
			[this] { SkipLevel(); },
			std::make_unique<KeyboardBindingInfo>(KeyboardKey::F1, KeyState::DownThisFrame)
		);
		//im.BindCommand(m_P1ActionMapIdx, "MoveRight", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::D, KeyState::Pressed));
		//im.BindCommand(m_P1ActionMapIdx, "Jump", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::W, KeyState::DownThisFrame));
		//im.BindCommand(m_P1ActionMapIdx, "Shoot", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::F, KeyState::DownThisFrame));

		im.BindCommand(m_P1ActionMapIdx, "MoveLeft", nullptr, std::make_unique<ControllerBindingInfo>(ControllerButton::DPAD_LEFT, ButtonState::Pressed));
		im.BindCommand(m_P1ActionMapIdx, "MoveRight", nullptr, std::make_unique<ControllerBindingInfo>(ControllerButton::DPAD_RIGHT, ButtonState::Pressed));
		im.BindCommand(m_P1ActionMapIdx, "Jump", nullptr, std::make_unique<ControllerBindingInfo>(ControllerButton::FACE_DOWN, ButtonState::DownThisFrame));
		im.BindCommand(m_P1ActionMapIdx, "Shoot", nullptr, std::make_unique<ControllerBindingInfo>(ControllerButton::FACE_LEFT, ButtonState::DownThisFrame));
		InputManager::GetInstance().SetEnableActionMap(m_P1ActionMapIdx, false);
	}
	void InGameState::OnEnter()
	{
		m_Score = 0;
		m_LevelIdx = 0;
		m_PlayerDied = false;
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

		InputManager::GetInstance().SetEnableActionMap(m_P1ActionMapIdx, true);
	}
	GameState InGameState::Update()
	{
		if (m_PlayerDied)
			return GameState::GameOverScreen;
		return GameState::None;
	}
	void InGameState::OnExit()
	{
		m_EnemyDiedConns.clear();
		InputManager::GetInstance().SetEnableActionMap(m_P1ActionMapIdx, false);
		m_pGameManagerComponent->PlayMusic(false);
	}
	void InGameState::HandlePlayerLostLive(JRE::EventInfo& e)
	{
		auto& args = e.GetArgs<Events::PlayerLostLive>();
		if (args.health <= 0)
			m_PlayerDied = true;
		else
			SetPlayerToSpawnPos(SceneManager::GetInstance().GetCurrentScene());
	}
	void InGameState::HandleEnemyDied(JRE::EventInfo& e)
	{
		auto& args = e.GetArgs<Events::EnemyDied>();
		++m_NrEnemiesKilled;
		m_Score += args.points;
		m_pScoreTxtCmp->SetText("Score: " + std::to_string(m_Score));

		if (m_NrEnemiesKilled >= m_NrEnemiesToKill)
		{
			SceneManager::OnSceneLoadCallBack loadCallback = [&](Scene& scene) -> void {
				SetPlayerToSpawnPos(scene);
				CreateEnemies(scene);
				};
			GoToNextLevel(loadCallback);
		}
	}
	void InGameState::SkipLevel()
	{
		SceneManager::OnSceneLoadCallBack loadCallback = [&](Scene& scene) -> void {
			SetPlayerToSpawnPos(scene);
			CreateEnemies(scene);
			};
		GoToNextLevel(loadCallback);
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

		m_NrEnemiesKilled = 0;

		sm.SetNextScene(levelName, loadCallback);
	}
	void InGameState::SetPlayerToSpawnPos(Scene& scene)
	{
		// Find player in gameObjects
		auto pPlayer = scene.GetGameObjectByComponentType<PlayerScriptComponent>();
		assert(pPlayer && "Couldn't find the player gameObject in the scene");
		// Find LevelData
		auto pLevelDataCmp = scene.GetComponent<LevelDataComponent>();
		// Set player spawnPos
		assert(pLevelDataCmp->m_LevelData->players.size() > 0 && "No player pos defined in the levelData");
		glm::vec2 spawnPos = pLevelDataCmp->m_LevelData->players[0];
		pPlayer->SetWorldPosition(spawnPos.x, spawnPos.y);
	}
	void InGameState::CreatePlayerAndUI(Scene& scene)
	{
		// Create players for gameMode
		auto pPlayer = std::make_unique<GameObject>("Bubby");
		PlayerBuilder()
			.SetAnimationPath("Anims/P1.txt")
			.SetActionMapIdx(m_P1ActionMapIdx)
			.Build(pPlayer);

		auto playerScriptCmp = pPlayer->GetComponent<PlayerScriptComponent>();
		m_PlayerLostLifeEventConn = playerScriptCmp->OnPlayerLostLive.AddObserver(
			[this](JRE::EventInfo& e) { HandlePlayerLostLive(e); });

		UIBuilder(scene)
			.SetPlayer1(*pPlayer)
			.Build();

		m_pScoreTxtCmp = scene.GetComponent<JRE::TextRendererComponent>();

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
		m_NrEnemiesToKill = static_cast<int>(enemies.size());
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
				auto zenchanScriptCmp = pZenchan->GetComponent<ZenchanScriptComponent>();
				m_EnemyDiedConns.push_back(zenchanScriptCmp->OnEnemyDied.AddObserver(
				[this](JRE::EventInfo& e) { HandleEnemyDied(e); }));
				scene.Add(std::move(pZenchan));
				break;
			}
			}
		}
	}
}
