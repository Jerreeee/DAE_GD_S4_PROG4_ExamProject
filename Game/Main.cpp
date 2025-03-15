#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextRendererComponent.h"
#include "SpriteRendererComponent.h"
#include "FPSComponent.h"
#include "RotateParentComponent.h"
#include <filesystem>
namespace fs = std::filesystem;

#include "GameCommands.h"
#include "GameComponents.h"


namespace Game
{
	void load()
	{
		auto pFont = Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

		auto& scene = Engine::SceneManager::GetInstance().CreateScene("Demo");

		auto go = std::make_unique<Engine::GameObject>("Background Image");
		if (auto pComponent = go->AddComponent<Engine::SpriteRendererComponent>(); pComponent)
		{
			pComponent->SetTexture("background.tga");
		}
		scene.Add(std::move(go));


		using namespace Engine::Input;
		InputManager& inputManager = InputManager::GetInstance();

		auto player1 = std::make_unique<Engine::GameObject>("Player1");
		player1->SetLocalPosition(216, 180);
		if (auto* pComponent = player1->AddComponent<Engine::SpriteRendererComponent>(); pComponent)
		{
			pComponent->SetTexture("logo.tga");
		}
		auto pPlayer1HealthComponentplayer1 = player1->AddComponent<HealthComponent>(3);
		auto takeDamageCommand = std::make_unique<TakeDamageCommand>(pPlayer1HealthComponentplayer1);
		auto moveUpCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ 0.f, -1.f });
		auto moveDownCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ 0.f, 1.f });
		auto moveLeftCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ -1.f, 0.f });
		auto moveRightCommand = std::make_unique<MoveCommand>(*player1.get(), 100.f, glm::vec2{ 1.f, 0.f });
		size_t player0Idx = inputManager.AddPlayer();
		inputManager.BindCommand(player0Idx, std::move(moveUpCommand), ControllerBindingInfo{ ControllerButton::DPAD_UP, ButtonState::Pressed })
			.BindCommand(player0Idx, std::move(moveDownCommand), ControllerBindingInfo{ ControllerButton::DPAD_DOWN, ButtonState::Pressed })
			.BindCommand(player0Idx, std::move(moveLeftCommand), ControllerBindingInfo{ ControllerButton::DPAD_LEFT, ButtonState::Pressed })
			.BindCommand(player0Idx, std::move(moveRightCommand), ControllerBindingInfo{ ControllerButton::DPAD_RIGHT, ButtonState::Pressed })
			.BindCommand(player0Idx, std::move(takeDamageCommand), ControllerBindingInfo{ ControllerButton::FACE_DOWN, ButtonState::DownThisFrame });
		scene.Add(std::move(player1));

		auto player2 = std::make_unique<Engine::GameObject>("Player 2");
		player2->SetLocalPosition(416, 180);
		if (auto* pComponent = player2->AddComponent<Engine::SpriteRendererComponent>(); pComponent)
		{
			pComponent->SetTexture("logo.tga");
		}
		auto moveUpCommand2 = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 0.f, -1.f });
		auto moveDownCommand2 = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 0.f, 1.f });
		auto moveLeftCommand2 = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ -1.f, 0.f });
		auto moveRightCommand2 = std::make_unique<MoveCommand>(*player2.get(), 200.f, glm::vec2{ 1.f, 0.f });
		size_t player1Idx = inputManager.AddPlayer();
		inputManager.BindCommand(player1Idx, std::move(moveUpCommand2), KeyboardBindingInfo{ KeyboardKey::W, KeyState::Pressed })
			.BindCommand(player1Idx, std::move(moveDownCommand2), KeyboardBindingInfo{ KeyboardKey::S, KeyState::Pressed })
			.BindCommand(player1Idx, std::move(moveLeftCommand2), KeyboardBindingInfo{ KeyboardKey::A, KeyState::Pressed })
			.BindCommand(player1Idx, std::move(moveRightCommand2), KeyboardBindingInfo{ KeyboardKey::D, KeyState::Pressed });
		scene.Add(std::move(player2));

		auto UI = std::make_unique<Engine::GameObject>("UI");
		UI->AddComponent<LivesComponentUI>(pPlayer1HealthComponentplayer1, pFont);
		scene.Add(std::move(UI));
	}

}
int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	Engine::Minigin engine(data_location);
	engine.Run(Game::load);
    return 0;
}
