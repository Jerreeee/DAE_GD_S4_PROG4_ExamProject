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
#include "GameCommands.h"

#include <filesystem>
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<dae::GameObject>("Background Image");
	if (auto pComponent = go->AddComponent<dae::SpriteRendererComponent>(); pComponent)
	{
		pComponent->SetTexture("background.tga");
	}
	scene.Add(std::move(go));


	using namespace dae::Input;
	InputManager& inputManager = InputManager::GetInstance();

	go = std::make_unique<dae::GameObject>("DAE Logo");
	go->SetLocalPosition(216, 180);
	if (auto* pComponent = go->AddComponent<dae::SpriteRendererComponent>(); pComponent)
	{
		pComponent->SetTexture("logo.tga");
	}
	auto moveUpCommand = std::make_unique<dae::MoveCommand>(*go.get(), 100.f, glm::vec2{0.f, -1.f});
	auto moveDownCommand = std::make_unique<dae::MoveCommand>(*go.get(), 100.f, glm::vec2{ 0.f, 1.f });
	auto moveLeftCommand = std::make_unique<dae::MoveCommand>(*go.get(), 100.f, glm::vec2{ -1.f, 0.f });
	auto moveRightCommand = std::make_unique<dae::MoveCommand>(*go.get(), 100.f, glm::vec2{ 1.f, 0.f });
	size_t player0Idx = inputManager.AddPlayer();
	inputManager.BindCommand(player0Idx, std::move(moveUpCommand), ControllerBindingInfo{ControllerButton::DPAD_UP, ButtonState::Pressed})
				.BindCommand(player0Idx, std::move(moveDownCommand), ControllerBindingInfo{ ControllerButton::DPAD_DOWN, ButtonState::Pressed })
				.BindCommand(player0Idx, std::move(moveLeftCommand), ControllerBindingInfo{ ControllerButton::DPAD_LEFT, ButtonState::Pressed })
				.BindCommand(player0Idx, std::move(moveRightCommand), ControllerBindingInfo{ ControllerButton::DPAD_RIGHT, ButtonState::Pressed });
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>("DAE Logo");
	go->SetLocalPosition(216, 180);
	if (auto* pComponent = go->AddComponent<dae::SpriteRendererComponent>(); pComponent)
	{
		pComponent->SetTexture("logo.tga");
	}
	auto moveUpCommand2 = std::make_unique<dae::MoveCommand>(*go.get(), 200.f, glm::vec2{ 0.f, -1.f });
	auto moveDownCommand2 = std::make_unique<dae::MoveCommand>(*go.get(), 200.f, glm::vec2{ 0.f, 1.f });
	auto moveLeftCommand2 = std::make_unique<dae::MoveCommand>(*go.get(), 200.f, glm::vec2{ -1.f, 0.f });
	auto moveRightCommand2 = std::make_unique<dae::MoveCommand>(*go.get(), 200.f, glm::vec2{ 1.f, 0.f });
	size_t player1Idx = inputManager.AddPlayer();
	inputManager.BindCommand(player1Idx, std::move(moveUpCommand2), KeyboardBindingInfo{ KeyboardKey::W, KeyState::Pressed })
				.BindCommand(player1Idx, std::move(moveDownCommand2), KeyboardBindingInfo{ KeyboardKey::S, KeyState::Pressed })
				.BindCommand(player1Idx, std::move(moveLeftCommand2), KeyboardBindingInfo{ KeyboardKey::A, KeyState::Pressed })
				.BindCommand(player1Idx, std::move(moveRightCommand2), KeyboardBindingInfo{ KeyboardKey::D, KeyState::Pressed });
	scene.Add(std::move(go));

	//go = std::make_unique<dae::GameObject>("DAE Logo 2");
	//go->SetLocalPosition(416, 280);
	//if (auto* pComponent = go->AddComponent<dae::SpriteRendererComponent>(); pComponent)
	//{
	//	pComponent->SetTexture("logo.tga");
	//}
	//auto moveUpCommand2 = std::make_unique<dae::MoveCommand>(*go.get(), 200.f, dae::Input::Direction::Up);
	//dae::Input::InputManager::GetInstance().BindCommand(dae::Input::Button::W, std::move(moveUpCommand2), dae::Input::KeyState::Pressed);
	//scene.Add(std::move(go));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
