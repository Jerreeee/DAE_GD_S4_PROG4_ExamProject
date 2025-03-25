#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <filesystem>
namespace fs = std::filesystem;

#include "JREngine/JREngine.h"
#include "JREngine/SceneManager.h"
#include "JREngine/InputManager.h"
#include "JREngine/ResourceManager.h"
#include "JREngine/GameObject.h"
#include "JREngine/Scene.h"
#include "JREngine/TextRendererComponent.h"
#include "JREngine/SpriteRendererComponent.h"

#include "FPSComponent.h"
#include "Commands.h"
#include "Components.h"

namespace BubbleBobble
{
	void load();
}

int main(int, char* [])
{
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	JREngine::JREngine engine(data_location);
	engine.Run(BubbleBobble::load);
	return 0;
}

namespace BubbleBobble
{
	void load()
	{
		auto pFont = JREngine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

		auto& scene = JREngine::SceneManager::GetInstance().CreateScene("Demo");

		auto go = std::make_unique<JREngine::GameObject>("Background Image");
		if (auto pComponent = go->AddComponent<JREngine::SpriteRendererComponent>(); pComponent)
		{
			pComponent->SetTexture("background.tga");
		}
		scene.Add(std::move(go));


		using namespace JREngine::Input;
		InputManager& inputManager = InputManager::GetInstance();

		//##################
		//Player 1
		//##################
		auto player1 = std::make_unique<JREngine::GameObject>("Player1");
		player1->SetLocalPosition(216, 180);
		if (auto* pComponent = player1->AddComponent<JREngine::SpriteRendererComponent>(); pComponent)
		{
			pComponent->SetTexture("logo.tga");
		}
		auto p1HealthComponent = player1->AddComponent<HealthComponent>(3);
		auto p1TakeDamageCommand = std::make_unique<TakeDamageCommand>(p1HealthComponent);
		auto p1ScoreComponent = player1->AddComponent<ScoreComponent>();
		auto p1IncreaseScoreCommand = std::make_unique<IncreaseScoreCommand>(p1ScoreComponent, 2);
		auto p1MoveUpCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ 0.f, -1.f });
		auto p1MoveDownCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ 0.f, 1.f });
		auto p1MoveLeftCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ -1.f, 0.f });
		auto p1MoveRightCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ 1.f, 0.f });
		size_t player0Idx = inputManager.AddPlayer();
		inputManager.BindCommand(player0Idx, std::move(p1MoveUpCommand), ControllerBindingInfo{ ControllerButton::DPAD_UP, ButtonState::Pressed })
					.BindCommand(player0Idx, std::move(p1MoveDownCommand), ControllerBindingInfo{ ControllerButton::DPAD_DOWN, ButtonState::Pressed })
					.BindCommand(player0Idx, std::move(p1MoveLeftCommand), ControllerBindingInfo{ ControllerButton::DPAD_LEFT, ButtonState::Pressed })
					.BindCommand(player0Idx, std::move(p1MoveRightCommand), ControllerBindingInfo{ ControllerButton::DPAD_RIGHT, ButtonState::Pressed })
					.BindCommand(player0Idx, std::move(p1TakeDamageCommand), ControllerBindingInfo{ ControllerButton::FACE_DOWN, ButtonState::DownThisFrame })
					.BindCommand(player0Idx, std::move(p1IncreaseScoreCommand), ControllerBindingInfo{ ControllerButton::FACE_UP, ButtonState::DownThisFrame });

		//##################
		//Player 2
		//##################
		auto player2 = std::make_unique<JREngine::GameObject>("Player 2");
		player2->SetLocalPosition(416, 180);
		if (auto* pComponent = player2->AddComponent<JREngine::SpriteRendererComponent>(); pComponent)
		{
			pComponent->SetTexture("logo.tga");
		}
		auto p2HealthComponent = player2->AddComponent<HealthComponent>(3);
		auto p2TakeDamageCommand = std::make_unique<TakeDamageCommand>(p2HealthComponent);
		auto p2ScoreComponent = player2->AddComponent<ScoreComponent>();
		auto p2IncreaseScoreCommand = std::make_unique<IncreaseScoreCommand>(p2ScoreComponent, 2);
		auto p2MoveUpCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 0.f, -1.f });
		auto p2MoveDownCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 0.f, 1.f });
		auto p2MoveLeftCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ -1.f, 0.f });
		auto p2MoveRightCommand = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 1.f, 0.f });
		size_t player1Idx = inputManager.AddPlayer();
		inputManager.BindCommand(player1Idx, std::move(p2MoveUpCommand), KeyboardBindingInfo{ KeyboardKey::W, KeyState::Pressed })
					.BindCommand(player1Idx, std::move(p2MoveDownCommand), KeyboardBindingInfo{ KeyboardKey::S, KeyState::Pressed })
					.BindCommand(player1Idx, std::move(p2MoveLeftCommand), KeyboardBindingInfo{ KeyboardKey::A, KeyState::Pressed })
					.BindCommand(player1Idx, std::move(p2MoveRightCommand), KeyboardBindingInfo{ KeyboardKey::D, KeyState::Pressed })
					.BindCommand(player1Idx, std::move(p2TakeDamageCommand), KeyboardBindingInfo{ KeyboardKey::F, KeyState::DownThisFrame })
					.BindCommand(player1Idx, std::move(p2IncreaseScoreCommand), KeyboardBindingInfo{ KeyboardKey::G, KeyState::DownThisFrame });

		//##################
		//UI
		//##################

		//Keybind info
		auto p1UIInfo = std::make_unique<JREngine::GameObject>();
		p1UIInfo->AddComponent<JREngine::TextRendererComponent>("Player1 | Move: DPAD | Damage: FACE_DOWN | Score: FACE_UP", pFont);
		auto p2UIInfo = std::make_unique<JREngine::GameObject>();
		p2UIInfo->AddComponent<JREngine::TextRendererComponent>("Player2 | Move: W,A,S,D | Damage: F | Score: G", pFont);

		//Player UI
		auto UI = std::make_unique<JREngine::GameObject>("UI");
		auto player1UI = std::make_unique<JREngine::GameObject>("player1UI");
		player1UI->AddComponent<PlayerUIComponent>(*(player1.get()), pFont);
		auto player2UI = std::make_unique<JREngine::GameObject>("player2UI");
		player2UI->AddComponent<PlayerUIComponent>(*(player2.get()), pFont);

		p1UIInfo->SetParent(UI.get());
		p2UIInfo->SetParent(UI.get());
		player1UI->SetParent(UI.get());
		player2UI->SetParent(UI.get());

		p1UIInfo->SetLocalPosition(0.f, 10.f);
		player1UI->SetLocalPosition(0.f, 40.f);
		p2UIInfo->SetLocalPosition(0.f, 120.f);
		player2UI->SetLocalPosition(0.f, 150.f);

		scene.Add(std::move(p1UIInfo));
		scene.Add(std::move(p2UIInfo));
		scene.Add(std::move(player1UI));
		scene.Add(std::move(player2UI));
		scene.Add(std::move(UI));

		scene.Add(std::move(player1));
		scene.Add(std::move(player2));
	}

}
