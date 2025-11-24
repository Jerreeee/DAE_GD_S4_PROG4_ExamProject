#pragma once
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <string>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Input/InputUtils.h"
#include "JREngine/Input/IKeyboard.h"
#include "JREngine/Input/IController.h"
#include "JREngine/Core/Command.h"

namespace JRE
{
namespace Input
{
	struct DeviceInfo
	{
		std::unique_ptr<IInputDevice> pDevice;
		bool polledThisTick;
	};

	struct Binding
	{
		std::unique_ptr<IBindingInfo> pBindInfo;
		Command command;
	};

	struct ActionMap
	{
		ActionMap();
		~ActionMap();
		ActionMap(const ActionMap&) = delete;
		ActionMap& operator=(const ActionMap&) = delete;
		ActionMap(ActionMap&&) noexcept = default;
		ActionMap& operator=(ActionMap&&) noexcept = default;

		bool Enabled() const { return m_Enabled; };
		const std::map<std::string, Binding>& GetBindings() const { return m_Bindings; };
	private:
		friend class InputManager;

		bool m_Enabled{ true };
		std::array<DeviceInfo*, 2> devicesInfo{}; //1 possible device per DeviceType
		std::map<std::string, Binding>  m_Bindings{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput();

		size_t AddKeyboard();
		size_t AddController();
		size_t AddActionMap(const std::vector<size_t>& deviceIndices);
		const ActionMap& GetActionMap(size_t actionMapIdx);
		InputManager& BindCommand(size_t actionMapIdx, const std::string& name, Command command, std::unique_ptr<IBindingInfo> pBindingInfo);
		bool IsBindingActive(const ActionMap& actionMap, const std::string& name);
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
