#pragma once
#include <vector>
#include <array>
#include <map>
#include <memory>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Input/InputUtils.h"
#include "JREngine/Input/IKeyboard.h"
#include "JREngine/Input/IController.h"
#include "JREngine/Core/Command.h"

namespace JRE
{
	class Command;
namespace Input
{
	struct DeviceInfo
	{
		std::unique_ptr<IInputDevice> pDevice;
		bool polledThisTick;
	};

	struct ActionMap
	{
		ActionMap();
		~ActionMap();
		ActionMap(const ActionMap&) = delete;
		ActionMap& operator=(const ActionMap&) = delete;
		ActionMap(ActionMap&&) noexcept = default;
		ActionMap& operator=(ActionMap&&) noexcept = default;

		bool enabled{ true };
		std::array<DeviceInfo*, 2> devicesInfo{}; //1 possible device per DeviceType
		std::vector<std::unique_ptr<Command>> m_Commands{};
		std::map<Command*, std::unique_ptr<IBindingInfo>> m_Bindings{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput();

		bool IsBindingActive(size_t actionMapIdx, const IBindingInfo& bindInfo);

		size_t AddKeyboard();
		size_t AddController();
		size_t AddActionMap(const std::vector<size_t>& deviceIndices);
		InputManager& BindCommand(size_t actionMapIdx, std::unique_ptr<Command> command, std::unique_ptr<IBindingInfo> pBindingInfo);
		void SetEnableActionMap(size_t actionMapIdx, bool enable);
	private:
		bool IsValidActionMapIdx(size_t idx);
		bool IsValidDeviceIdx(size_t idx);

		int m_NrControllers{};
		std::vector<bool> m_PolledDevices{};
		std::vector<DeviceInfo> m_Devices{};
		std::vector<ActionMap> m_ActionMaps{};
	};
}}
