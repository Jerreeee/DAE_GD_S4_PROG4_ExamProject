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
			if (!actionMap.m_Enabled)
				continue;

			//Check if the devices for this acionMap were already Polled this Update tick
			for (DeviceInfo* pDeviceInfo : actionMap.devicesInfo)
				if (pDeviceInfo && !pDeviceInfo->polledThisTick)
					pDeviceInfo->pDevice->PollState();

			for (const auto& [name, binding] : actionMap.m_Bindings)
			{
				if (!binding.command) continue;
				auto& pDevice = actionMap.devicesInfo[static_cast<size_t>(binding.pBindInfo->GetType())]->pDevice;
				if (pDevice->IsBindingActive(*binding.pBindInfo))
					binding.command();
			}
		}

		return true;
	}
	bool InputManager::IsBindingActive(const ActionMap& actionMap, const std::string& name)
	{
		auto it = actionMap.m_Bindings.find(name);
		if (it == actionMap.m_Bindings.end())
			return false;
		const Binding& binding = it->second;
		auto& pDevice = actionMap.devicesInfo[static_cast<size_t>(binding.pBindInfo->GetType())]->pDevice;
		return pDevice->IsBindingActive(*binding.pBindInfo);
	}
	void InputManager::SetEnableActionMap(size_t actionMapIdx, bool enable)
	{
		assert(IsValidActionMapIdx(actionMapIdx) && "Invalid actionMapIdx");
		m_ActionMaps[actionMapIdx].m_Enabled = enable;
	}
	size_t InputManager::AddKeyboard()
	{
		m_Devices.emplace_back(std::make_unique<SDLKeyboard>());
		return m_Devices.size() - 1;
	}
	size_t InputManager::AddController()
	{
		m_Devices.emplace_back(std::make_unique<XBoxController>(m_NrControllers));
		++m_NrControllers;
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
	const ActionMap& InputManager::GetActionMap(size_t actionMapIdx)
	{
		assert(IsValidActionMapIdx(actionMapIdx) && "InputManager::GetActionMap | Invalid actionMap idx");
		return m_ActionMaps[actionMapIdx];
	}
	InputManager& InputManager::BindCommand(size_t actionMapIdx, const std::string& name, Command command, std::unique_ptr<IBindingInfo> pBindingInfo)
	{
		if (!IsValidActionMapIdx(actionMapIdx))
			return *this;
		auto& actionMap = m_ActionMaps[actionMapIdx];

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

		actionMap.m_Bindings[name] = Binding{ std::move(pBindingInfo),std::move(command) };
		return *this;
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
