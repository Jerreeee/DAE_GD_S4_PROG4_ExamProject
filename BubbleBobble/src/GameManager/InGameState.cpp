#include <filesystem>
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Input/InputManager.h"

#include "Player/PlayerScriptComponent.h"
#include "SceneBuilders/LevelBuilder.h"
#include "GameManagerComponent.h"
#include "Assets/LevelDataComponent.h"
#include "Player/PlayerBuilder.h"
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
		auto& im = InputManager::GetInstance();

		GameMode mode = m_GameManagerComponent.GetGameMode();

		mode;
		//Create actionMap for the player and set bindings
		size_t actionMapIdx = im.AddActionMap({ 0 });
		const ActionMap& actionMap = im.GetActionMap(actionMapIdx);
		im.BindCommand(actionMapIdx, "MoveLeft", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::Q, KeyState::DownThisFrame));
		im.BindCommand(actionMapIdx, "MoveRight", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::D, KeyState::DownThisFrame));
		im.BindCommand(actionMapIdx, "Jump", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::W, KeyState::DownThisFrame));
		im.BindCommand(actionMapIdx, "Shoot", nullptr, std::make_unique<KeyboardBindingInfo>(KeyboardKey::F, KeyState::DownThisFrame));

		//Create player(s) for gameMode
		auto player = std::make_unique<GameObject>();
		Player::Builder()
			.SetAnimationPath("Anims/P1.txt")
			.SetActionMap(actionMap)
			.Build(player);

		//Load 1st level
		GoToNextLevel();
		//Add player to the current level
		SceneManager::GetInstance().GetCurrentScene().Add(std::move(player));
		SetPlayerToSpawnPos();
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

		// 1) Find player in gameObjects (with tag, or PlayerScriptComponent)
		auto pPlayer = scene.GetGameObjectByComponentType<Player::ScriptComponent>();
		assert(pPlayer && "Couldn't find the player gameObject in the scene");
		// 2) Find LevelData
		auto pLevelDataCmp = scene.GetComponent<LevelDataComponent>();
		// 3) Set player pos to spawn pos
		assert(pLevelDataCmp->m_LevelData->players.size() > 0 && "No player pos defined in the levelData");
		glm::vec2 spawnPos = pLevelDataCmp->m_LevelData->players[0];
		pPlayer->SetWorldPosition(spawnPos.x, spawnPos.y);

		// 4) Load enemy spawn positions from file
		// 5) spawn enemies at positions
	}
}
