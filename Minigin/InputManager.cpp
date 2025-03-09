#include <SDL.h>
#include "InputManager.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <iostream>
namespace dae::Input
{
	InputManager::InputManager()
	{
		m_pController = std::make_unique<Controller>();
		m_pKeyboard = std::make_unique<Keyboard>();
		m_pKeyboard->AddKeysToTrack({Button::W, Button::A, Button::S, Button::D});
	}

	bool InputManager::ProcessInput()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				return false;
			}
			if (e.type == SDL_KEYDOWN) {

			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {

			}
			ImGui_ImplSDL2_ProcessEvent(&e);
		}

		m_pController->PollState(0);
		//m_pKeyboard->PollState();
		for (const auto& [button, commandBinding] : m_ButtonCommandMap)
		{
			if (BUTTON_GAMEPAD_MASK.test(static_cast<size_t>(button)) &&
				m_pController->HasKeyState(static_cast<uint32_t>(button), commandBinding.keyState))
			{
				commandBinding.command->Execute();
			}
			//else if (BUTTON_KEYBOARD_MASK.test(static_cast<size_t>(button)) &&
			//	m_pKeyboard->HasKeyState(static_cast<uint32_t>(button), commandBinding.keyState))
			//{
			//	commandBinding.command->Execute();
			//}
		}

		return true;
	}
	void InputManager::BindCommand(Button button, std::unique_ptr<Command> command, KeyState keyState)
	{
		m_ButtonCommandMap.insert({ button, Command::Binding{std::move(command), keyState}});
	}
}
