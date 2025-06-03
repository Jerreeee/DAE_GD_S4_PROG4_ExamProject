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

	//#include "Player/PlayerBuilder.h"
	#include "GameManager/GameManagerComponent.h"
	#include "MenuBuilders.h"

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

			auto& sm = SceneManager::GetInstance();
			auto& mainMenuScene = sm.CreateScene(gameManagerCmp->GetStateName(GameState::MainMenu));
			MainMenuBuilder(mainMenuScene).Build();
			mainMenuScene.Add(std::move(gameManager));

			auto& loadingMenuScene = sm.CreateScene(gameManagerCmp->GetStateName(GameState::LoadingScreen));
			LoadingMenuBuilder(loadingMenuScene).Build();

			sm.SetStartSceneName(gameManagerCmp->GetStateName(GameState::MainMenu));

			//JRE::Input::InputManager& inputManager = JRE::Input::InputManager::GetInstance();

			/*
			Loading a sceen:
			1) game obejct + tilemap
			2) add player (from previous level)
			*/


			//##################
			//Player 1
			//##################
			//auto player1 = std::make_unique<JRE::GameObject>("Player1");
			//Player::Builder()
			//	.SetAnimationPath("Data/Anims/P1.txt")
			//	.Build(player1);
			//scene.Add(std::move(player1));
		}
	}
