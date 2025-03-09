#include <SDL.h>
#include "InputManager.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <iostream>
namespace dae::Input
{
	InputManager::InputManager()
		//m_ButtonCommandMap()
	{
		m_pController = std::make_unique<Controller>();
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
		for (const auto& buttonCommandPair : m_ButtonCommandMap)
		{
			const Button& button = buttonCommandPair.first;
			const Command::Binding& commandBinding = buttonCommandPair.second;
			if (BUTTON_GAMEPAD_MASK.test(static_cast<size_t>(button)) &&
				m_pController->HasKeyState(static_cast<unsigned int>(button), commandBinding.keyState))
			{
				commandBinding.command->Execute();
			}
		}

		return true;
	}
	void InputManager::BindCommand(Button button, std::unique_ptr<Command> command, KeyState keyState)
	{
		m_ButtonCommandMap.insert({ button, Command::Binding{std::move(command), keyState}});
	}
}
