#include <iostream>
#include <SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "Input/XBoxController.h"
#include "Input/SDLKeyboard.h"
#include "Input/InputManager.h"

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

		//Reset all devices polled state
		std::fill(m_PolledDevices.begin(), m_PolledDevices.end(), false);

		for (const ActionMap& actionMap : m_ActionMaps)
		{
			if (!actionMap.enabled)
				continue;

			//Check if the devices for this acionMap were already Polled this Update tick
			for (DeviceInfo* pDeviceInfo : actionMap.devicesInfo)
				if (pDeviceInfo && !pDeviceInfo->polledThisTick)
					pDeviceInfo->pDevice->PollState();

			for (const auto& [pCommand, pBindInfo] : actionMap.m_Bindings)
			{
				auto& pDevice = actionMap.devicesInfo[static_cast<size_t>(pBindInfo->GetType())]->pDevice;
				if (pDevice->IsBindingActive(*pBindInfo))
					pCommand->Execute();
			}
		}

		return true;
	}
	bool InputManager::IsBindingActive(size_t actionMapIdx, const IBindingInfo& bindInfo)
	{
		if (!IsValidActionMapIdx(actionMapIdx))
			return false;
		auto& pDevice = m_ActionMaps[actionMapIdx].devicesInfo[static_cast<size_t>(bindInfo.GetType())]->pDevice;
		return pDevice->IsBindingActive(bindInfo);
	}
	InputManager& InputManager::BindCommand(size_t actionMapIdx, std::unique_ptr<Command> command, std::unique_ptr<IBindingInfo> pBindingInfo)
	{
		assert(IsValidActionMapIdx(actionMapIdx) && "Invalid actionMapIdx");
		ActionMap& actionMap = m_ActionMaps[actionMapIdx];
		actionMap.m_Commands.emplace_back(std::move(command));
		Command* pCommand = actionMap.m_Commands[actionMap.m_Commands.size() - 1].get();

		//Add keys to track
		if (pBindingInfo->GetType() == DeviceType::Keyboard)
		{
			DeviceInfo* pDeviceInfo = actionMap.devicesInfo[static_cast<size_t>(DeviceType::Keyboard)];
			if (pDeviceInfo)
			{
				auto pKeyboard = static_cast<IKeyboard*>(pDeviceInfo->pDevice.get());
				auto pKeyboardBindingInfo = static_cast<KeyboardBindingInfo*>(pBindingInfo.get());
				pKeyboard->AddKeysToTrack({ pKeyboardBindingInfo->key });
			}
		}

		actionMap.m_Bindings.insert({ pCommand, std::move(pBindingInfo) });
		return *this;
	}
	void InputManager::SetEnableActionMap(size_t actionMapIdx, bool enable)
	{
		assert(IsValidActionMapIdx(actionMapIdx) && "Invalid actionMapIdx");
		m_ActionMaps[actionMapIdx].enabled = enable;
	}
	size_t InputManager::AddKeyboard()
	{
		m_Devices.emplace_back(std::make_unique<SDLKeyboard>());
		return m_Devices.size() - 1;
	}
	size_t InputManager::AddController()
	{
		++m_NrControllers;
		m_Devices.emplace_back(std::make_unique<XBoxController>(m_NrControllers));
		return m_Devices.size() - 1;
	}
	size_t InputManager::AddActionMap(const std::vector<size_t>& deviceIndices)
	{
		m_ActionMaps.emplace_back(ActionMap{});
		size_t idx = m_ActionMaps.size() - 1;
		ActionMap& actionMap = m_ActionMaps[idx];

		//Assign devices to actionMap
		for (int i{}; i < deviceIndices.size() && i < actionMap.devicesInfo.size(); ++i)
		{
			DeviceInfo& deviceInfo = m_Devices[deviceIndices[i]];
			size_t typeIdx = static_cast<size_t>(deviceInfo.pDevice->GetType());
			actionMap.devicesInfo[typeIdx] = &deviceInfo;
		}

		return idx;
	}
	bool InputManager::IsValidActionMapIdx(size_t idx)
	{
		return idx >= 0 && idx < m_ActionMaps.size();
	}
	bool InputManager::IsValidDeviceIdx(size_t idx)
	{
		return idx >= 0 && idx < m_Devices.size();
	}

	ActionMap::ActionMap() = default;
	ActionMap::~ActionMap() = default;
}
