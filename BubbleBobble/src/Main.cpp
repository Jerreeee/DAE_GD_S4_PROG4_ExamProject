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
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Input/InputManager.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Audio/ISoundSystem.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Scene/GameObject.h"

#include "JREngine/Audio/ISoundClip.h"
#include "JREngine/Asset/Font.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/Sprite.h"

#include "JREngine/Rendering/TextRendererComponent.h"

#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/SoundClipImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/FontImporter.h"

#include "Player/Player.h"

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

namespace BubbleBobble
{
	void load()
	{
		//auto hitSoundHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::SoundClipImporter("HUD/Select.wav")));
		//auto softHitSoundRef = JRE::SoftAssetRef<JRE::ISoundClip>(hitSoundHandle);
		//auto fontHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::FontImporter("Lingua.otf").SetSize(20)));
		//auto softFontRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

		auto& scene = JRE::SceneManager::GetInstance().CreateScene("Demo");

		//JRE::Input::InputManager& inputManager = JRE::Input::InputManager::GetInstance();

		//##################
		//Player 1
		//##################
		auto player1 = std::make_unique<JRE::GameObject>("Player1");
		Player::Builder()
			.SetAnimationPath("Data/Anims/P1.txt")
			.Build(player1);
		scene.Add(std::move(player1));

		//##################
		//Player 2
		//##################
		//auto player2 = std::make_unique<JRE::GameObject>("Player 2");
		//player2->SetLocalPosition(416, 180);
		//if (auto* pComponent = player2->AddComponent<JRE::SpriteRendererComponent>(); pComponent)
		//{
		//	JRE::AssetHandle texHandle = JRE::ServiceLocator::GetResourceManager().LoadTexture("Player/Bobby/Bubble_Anim.png");
		//	pComponent->SetSprite(texHandle);
		//}
		//auto p2HealthComponent = player2->AddComponent<HealthComponent>(3);
		//p2HealthComponent->SetHitSound(softHitSoundRef);
		//auto p2TakeDamageCommand = std::make_unique<TakeDamageCommand>(p2HealthComponent);
		//auto p2ScoreComponent = player2->AddComponent<ScoreComponent>();
		//auto p2IncreaseScoreCommand = std::make_unique<IncreaseScoreCommand>(p2ScoreComponent, 2);
		//auto p2MoveUpCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 0.f, -1.f });
		//auto p2MoveDownCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 0.f, 1.f });
		//auto p2MoveLeftCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ -1.f, 0.f });
		//auto p2MoveRightCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 1.f, 0.f });
		//size_t player1Idx = inputManager.AddPlayer();
		//inputManager.BindCommand(player1Idx, std::move(p2MoveUpCommand), JRE::Input::KeyboardBindingInfo{ JRE::Input::KeyboardKey::W, JRE::Input::KeyState::Pressed })
		//			.BindCommand(player1Idx, std::move(p2MoveDownCommand), JRE::Input::KeyboardBindingInfo{ JRE::Input::KeyboardKey::S, JRE::Input::KeyState::Pressed })
		//			.BindCommand(player1Idx, std::move(p2MoveLeftCommand), JRE::Input::KeyboardBindingInfo{ JRE::Input::KeyboardKey::A, JRE::Input::KeyState::Pressed })
		//			.BindCommand(player1Idx, std::move(p2MoveRightCommand), JRE::Input::KeyboardBindingInfo{ JRE::Input::KeyboardKey::D, JRE::Input::KeyState::Pressed })
		//			.BindCommand(player1Idx, std::move(p2TakeDamageCommand), JRE::Input::KeyboardBindingInfo{ JRE::Input::KeyboardKey::F, JRE::Input::KeyState::DownThisFrame })
		//			.BindCommand(player1Idx, std::move(p2IncreaseScoreCommand), JRE::Input::KeyboardBindingInfo{ JRE::Input::KeyboardKey::G, JRE::Input::KeyState::DownThisFrame });

		//##################
		//UI
		//##################

		//Keybind info
		//auto p1UIInfo = std::make_unique<JRE::GameObject>();
		//p1UIInfo->AddComponent<JRE::TextRendererComponent>("Player1 | Move: DPAD (state will change)", softFontRef);
		//auto p2UIInfo = std::make_unique<JRE::GameObject>();
		//p2UIInfo->AddComponent<JRE::TextRendererComponent>("Player2 | Move: W,A,S,D | Damage: F (plays sound) | Score: G", softFontRef);

		//Player UI
		//auto UI = std::make_unique<JRE::GameObject>("UI");
		//auto player1UI = std::make_unique<JRE::GameObject>("player1UI");
		//player1UI->AddComponent<PlayerUIComponent>(*(player1.get()), softFontRef);
		//auto player2UI = std::make_unique<JRE::GameObject>("player2UI");
		//player2UI->AddComponent<PlayerUIComponent>(*(player2.get()), softFontRef);

		//p1UIInfo->SetParent(UI.get());
		//p2UIInfo->SetParent(UI.get());
		//player1UI->SetParent(UI.get());
		//player2UI->SetParent(UI.get());

		//p1UIInfo->SetLocalPosition(0.f, 10.f);
		//player1UI->SetLocalPosition(0.f, 40.f);
		//p2UIInfo->SetLocalPosition(0.f, 120.f);
		//player2UI->SetLocalPosition(0.f, 150.f);

		//scene.Add(std::move(p1UIInfo));
		//scene.Add(std::move(p2UIInfo));
		//scene.Add(std::move(player1UI));
		//scene.Add(std::move(player2UI));
		//scene.Add(std::move(UI));

		//scene.Add(std::move(player1));
		//scene.Add(std::move(player2));
	}
}
