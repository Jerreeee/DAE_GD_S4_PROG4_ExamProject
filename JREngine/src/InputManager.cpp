#include <SDL.h>
#include "InputManager.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include <iostream>
#include "XBoxController.h"
#include "SDLKeyboard.h"

namespace JREngine::Input
{
	InputManager::InputManager() = default;
	InputManager::~InputManager() = default;

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

		int controllerIdx{};
		for (size_t playerIdx{}; playerIdx < m_PlayerInputBindings.size(); ++playerIdx)
		{
			const auto& playerInputBinding = m_PlayerInputBindings[playerIdx];
			if (playerInputBinding.pController.get())
			{
				playerInputBinding.pController->PollState(static_cast<int>(controllerIdx));
				for (const auto& [pCommand, bindInfo] : playerInputBinding.controllerBindings)
					if (playerInputBinding.pController->HasButtonState(bindInfo.button, bindInfo.buttonState))
						pCommand->Execute();
				++controllerIdx;
			}
			if (playerInputBinding.pKeyboard.get())
			{
				playerInputBinding.pKeyboard->PollState();
				for (const auto& [pCommand, bindInfo] : playerInputBinding.keyboardBindings)
					if (playerInputBinding.pKeyboard->HasKeyState(bindInfo.key, bindInfo.keyState))
						pCommand->Execute();
			}
		}

		return true;
	}
	size_t InputManager::AddPlayer()
	{
		m_PlayerInputBindings.emplace_back(PlayerInputBindingsInfo{});
		return m_PlayerInputBindings.size() - 1;
	}
	InputManager& InputManager::BindCommand(size_t playerIdx, std::unique_ptr<Command> command, KeyboardBindingInfo bindInfo)
	{
		if (!IsValidPlayerIdx(playerIdx))
			return *this;
		PlayerInputBindingsInfo& info = m_PlayerInputBindings[playerIdx];
		info.commands.emplace_back(std::move(command));
		Command* pCommand = info.commands[info.commands.size() - 1].get();
		info.keyboardBindings.insert({ pCommand, bindInfo });
		if (!m_PlayerInputBindings[playerIdx].pKeyboard.get())
			m_PlayerInputBindings[playerIdx].pKeyboard = std::make_unique<SDLKeyboard>();
		m_PlayerInputBindings[playerIdx].pKeyboard->AddKeysToTrack({ bindInfo.key });
		return *this;
	}
	InputManager& InputManager::BindCommand(size_t playerIdx, std::unique_ptr<Command> command, ControllerBindingInfo bindInfo)
	{
		if (!IsValidPlayerIdx(playerIdx))
			return *this;
		PlayerInputBindingsInfo& info = m_PlayerInputBindings[playerIdx];
		info.commands.emplace_back(std::move(command));
		Command* pCommand = info.commands[info.commands.size() - 1].get();
		info.controllerBindings.insert({ pCommand, bindInfo });
		if (!m_PlayerInputBindings[playerIdx].pController.get())
			m_PlayerInputBindings[playerIdx].pController = std::make_unique<XBoxController>();
		return *this;
	}
	bool InputManager::IsValidPlayerIdx(size_t playerIdx)
	{
		return playerIdx >= 0 && playerIdx < m_PlayerInputBindings.size();
	}
	PlayerInputBindingsInfo::PlayerInputBindingsInfo() = default;
	PlayerInputBindingsInfo::~PlayerInputBindingsInfo() = default;
}
