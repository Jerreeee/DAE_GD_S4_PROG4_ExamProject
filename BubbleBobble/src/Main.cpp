#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

#include "JREngine/JREngine.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Asset/AssetRegistry.h"
#include "JREngine/Asset/RuntimeResourceManager.h"

#include "EngineSetup.h"
#include "GameManager/GameManagerComponent.h"
#include "SceneBuilders/MainMenuBuilder.h"
#include "SceneBuilders/LoadingScreenBuilder.h"
#include "SceneBuilders/LevelBuilder.h"
#include "SceneBuilders/GameOverScreenBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	void buildScenes()
	{
		auto gameManager = std::make_unique<JRE::GameObject>("GameManager");
		auto gameManagerCmp = gameManager->AddComponent<GameManagerComponent>(GameState::MainMenu);
		gameManager->m_PersistenceScope = PersistenceLayer::Global;

		auto& sm = SceneManager::GetInstance();
		auto& mainMenuScene = sm.CreateScene(gameManagerCmp->GetStateName(GameState::MainMenu), PersistenceMask::MenuScene);
		MainMenuBuilder(mainMenuScene).Build();
		mainMenuScene.Add(std::move(gameManager));

		auto& loadingMenuScene = sm.CreateScene(gameManagerCmp->GetStateName(GameState::LoadingScreen), PersistenceMask::MenuScene);
		LoadingMenuBuilder(loadingMenuScene).Build();
		auto& gameOverMenuScene = sm.CreateScene(gameManagerCmp->GetStateName(GameState::GameOverScreen), PersistenceMask::MenuScene);
		GameOverScreenBuilder(gameOverMenuScene).Build();

		sm.SetNextScene(gameManagerCmp->GetStateName(GameState::MainMenu));
	}
}

int main(int, char* [])
{
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	const auto manifestPath = data_location / "asset_manifest.txt";
	if (!fs::exists(manifestPath))
	{
		std::cerr << "[BubbleBobble] Asset manifest not found: " << manifestPath << "\n"
		          << "Run JREditor.exe first to generate it.\n";
		return 1;
	}

	JRE::JREngine engine{};
	ServiceLocator::RegisterResourceManager(std::make_unique<RuntimeResourceManager>(manifestPath));

	engine.Run(BubbleBobble::buildScenes);
	return 0;
}
