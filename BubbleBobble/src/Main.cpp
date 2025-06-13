#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <filesystem>
namespace fs = std::filesystem;
#include <iostream>

#include "JREngine/JREngine.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/GameObject.h"


#include "EngineSetup.h"
#include "GameManager/GameManagerComponent.h"
#include "SceneBuilders/MainMenuBuilder.h"
#include "SceneBuilders/LoadingScreenBuilder.h"
#include "SceneBuilders/LevelBuilder.h"
#include "SceneBuilders/GameOverScreenBuilder.h"

namespace BubbleBobble
{
	void load();
}

int main(int, char* [])
{
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	JRE::JREngine engine(data_location);
	engine.Run(BubbleBobble::load);
	return 0;
}

using namespace JRE;

namespace BubbleBobble
{
	void load()
	{
		//auto hitSoundHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::SoundClipImporter("HUD/Select.wav")));
		//auto softHitSoundRef = JRE::SoftAssetRef<JRE::ISoundClip>(hitSoundHandle);
		//auto fontHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::FontImporter("Lingua.otf").SetSize(20)));
		//auto softFontRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

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
