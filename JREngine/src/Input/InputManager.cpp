#include <iostream>
#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "Input/InputManager.h"
#include "Input/XBoxController.h"
#include "Input/SDLKeyboard.h"

namespace JRE::Input
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

		for (size_t actionMapIdx{}; actionMapIdx < m_ActionMaps.size(); ++actionMapIdx)
		{
			const auto& actionMap = m_ActionMaps[actionMapIdx];
			if (!actionMap.enabled)
				continue;

			for (const auto& [pCommand, bindInfo] : actionMap.bindings)
				if (actionMap.pDevice->IsBindingActive(*bindInfo))
					pCommand->Execute();
		}

		return true;
	}
	bool InputManager::IsBindingActive(size_t actionMapIdx, const IBindingInfo& bindInfo)
	{
		if (!IsValidActionMapIdx(actionMapIdx))
			return false;
		return m_ActionMaps[actionMapIdx].pDevice->IsBindingActive(bindInfo);
	}
	size_t InputManager::AddKeyboard()
	{
		m_Devices.emplace_back(std::make_unique<SDLKeyboard>());
		return m_Devices.size() - 1;
	}
	size_t InputManager::AddController()
	{
		m_Devices.emplace_back(std::make_unique<XBoxController>());
		return m_Devices.size() - 1;
	}
	size_t InputManager::AddPlayer(size_t deviceIdx)
	{
		if (!IsValidDeviceIdx(deviceIdx))
			throw std::runtime_error("Invalid deviceIdx");
		m_Players.emplace_back(PlayerInput{});
		size_t idx = m_Players.size() - 1;
		m_Players[idx].pDevice = m_Devices[deviceIdx].get();
		return idx;
	}
	size_t InputManager::AddActionMap(size_t playerIdx)
	{
		if (!IsValidPlayerIdx(playerIdx))
			throw std::runtime_error("Invalid playerIdx");
		m_ActionMaps.emplace_back(ActionMap{});
		size_t idx = m_ActionMaps.size() - 1;
		m_ActionMaps[idx].pDevice = m_Players[playerIdx].pDevice;
		return idx;
	}
	ActionMap& InputManager::GetActionMap(size_t actionMapIdx)
	{
		assert(IsValidActionMapIdx(actionMapIdx) && "Invalid actionMapIdx");
		return m_ActionMaps[actionMapIdx];
	}
	bool InputManager::IsValidActionMapIdx(size_t idx)
	{
		return idx >= 0 && idx < m_ActionMaps.size();
	}
	bool InputManager::IsValidDeviceIdx(size_t idx)
	{
		return idx >= 0 && idx < m_Devices.size();
	}

	bool InputManager::IsValidPlayerIdx(size_t idx)
	{
		return idx >= 0 && idx < m_Players.size();
	}

	ActionMap::ActionMap() = default;
	ActionMap::~ActionMap() = default;
	ActionMap& ActionMap::BindCommand(std::unique_ptr<Command> command, std::unique_ptr<IBindingInfo> pBindingInfo)
	{
		assert(pBindingInfo->GetType() == pDevice->GetType() && "BindingInfo DeviceType doesnt match ActionMap DeviceType");

		commands.emplace_back(std::move(command));
		Command* pCommand = commands[commands.size() - 1].get();
		bindings.insert({ pCommand, std::move(pBindingInfo) });

		//If its a keyboard add the key to track
		if (pBindingInfo->GetType() == DeviceType::Keyboard)
		{
			auto pKeyboard = static_cast<IKeyboard*>(pDevice);
			auto pKeyboardBindingInfo = static_cast<KeyboardBindingInfo*>(pBindingInfo.get());
			pKeyboard->AddKeysToTrack({ pKeyboardBindingInfo->key });
		}

		return *this;
	}
}
