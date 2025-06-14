#include <filesystem>
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

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	InGameState::InGameState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "InGame")
	{
		auto& im = InputManager::GetInstance();

		//Create actionMap for the player and set bindings
		m_P1ActionMapIdx = im.AddActionMap({ 0 });
		im.BindCommand(m_P1ActionMapIdx, "MoveLeft", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::A, KeyState::Pressed));
		im.BindCommand(m_P1ActionMapIdx, "MoveRight", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::D, KeyState::Pressed));
		im.BindCommand(m_P1ActionMapIdx, "Jump", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::W, KeyState::DownThisFrame));
		im.BindCommand(m_P1ActionMapIdx, "Shoot", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::F, KeyState::DownThisFrame));
		InputManager::GetInstance().SetEnableActionMap(m_P1ActionMapIdx, false);
	}
	InGameState::~InGameState()
	{
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
		InputManager::GetInstance().SetEnableActionMap(m_P1ActionMapIdx, false);
	}
	void InGameState::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case JRE::Events::EventDestroyed::ID:
		{
			auto& args = event.GetArgs<JRE::Events::EventDestroyed>();
			args.event.RemoveObserver(this);
			break;
		}
		case Events::PlayerLostLive::ID:
		{
			auto& args = event.GetArgs<Events::PlayerLostLive>();
			if (args.health <= 0)
				m_PlayerDied = true;
			else
			{
				SetPlayerToSpawnPos(SceneManager::GetInstance().GetCurrentScene());
			}
			break;
		}
		case Events::EnemyDied::ID:
		{
			auto& args = event.GetArgs<Events::EnemyDied>();
			++m_NrEnemiesKilled;
			m_Score += args.points;
			m_pScoreTxtCmp->SetText("Score: " + std::to_string(m_Score));

			//Go to the next level when all enemies are dead
			if (m_NrEnemiesKilled >= m_NrEnemiesToKill)
			{
				SceneManager::OnSceneLoadCallBack loadCallback = [&](Scene& scene) -> void {
					SetPlayerToSpawnPos(scene);
					CreateEnemies(scene);
					};
				GoToNextLevel(loadCallback);
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
		//Create player(s) for gameMode
		auto pPlayer = std::make_unique<GameObject>("Bubby");
		PlayerBuilder()
			.SetAnimationPath("Anims/P1.txt")
			.SetActionMapIdx(m_P1ActionMapIdx)
			.Build(pPlayer);

		auto playerScriptCmp = pPlayer->GetComponent<PlayerScriptComponent>();
		playerScriptCmp->OnPlayerLostLive.AddObserver(this);

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
				zenchanScriptCmp->OnEnemyDied.AddObserver(this);
				scene.Add(std::move(pZenchan));
				break;
			}
			}
		}
	}
}
