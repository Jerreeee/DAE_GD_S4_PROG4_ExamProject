#pragma once
#include <vector>
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
	struct PlayerInput
	{
	public:
		std::vector<size_t> actionMapIndices; //Indices into m_ActionMaps
	private:
		friend class InputManager;

		IInputDevice* pDevice{};
	};

	class InputManager;
	struct ActionMap final
	{
	public:
		ActionMap();
		~ActionMap();
		ActionMap(const ActionMap&) = delete;
		ActionMap& operator=(const ActionMap&) = delete;
		ActionMap(ActionMap&&) noexcept = default;
		ActionMap& operator=(ActionMap&&) noexcept = default;

		ActionMap& BindCommand(std::unique_ptr<Command> command, std::unique_ptr<IBindingInfo> pBindingInfo);

		bool enabled{ true };
	private:
		friend class InputManager;

		IInputDevice* pDevice{};
		std::vector<std::unique_ptr<Command>> commands{};
		std::map<Command*, std::unique_ptr<IBindingInfo>> bindings{};
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
		size_t AddPlayer(size_t deviceIdx);
		size_t AddActionMap(size_t playerIdx);
		ActionMap& GetActionMap(size_t actionMapIdx);
	private:
		bool IsValidActionMapIdx(size_t idx);
		bool IsValidDeviceIdx(size_t idx);
		bool IsValidPlayerIdx(size_t idx);

		std::vector<std::unique_ptr<IInputDevice>> m_Devices{};
		std::vector<PlayerInput> m_Players{};
		std::vector<ActionMap> m_ActionMaps{};
	};
}}
