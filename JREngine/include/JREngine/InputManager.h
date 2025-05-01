#pragma once
#include <vector>
#include <map>
#include "Singleton.h"
#include "InputUtils.h"
#include <memory>
#include "Command.h"

namespace JRE
{
	class Command;
namespace Input
{
	class IController;
	class Keyboard;

	struct KeyboardBindingInfo
	{
		KeyboardKey key;
		KeyState keyState;
	};

	struct ControllerBindingInfo
	{
		ControllerButton button;
		ButtonState buttonState;
	};

	struct PlayerInputBindingsInfo final
	{
		PlayerInputBindingsInfo();
		~PlayerInputBindingsInfo();
		PlayerInputBindingsInfo(const PlayerInputBindingsInfo&) = delete;
		PlayerInputBindingsInfo& operator=(const PlayerInputBindingsInfo&) = delete;
		PlayerInputBindingsInfo(PlayerInputBindingsInfo&&) noexcept = default;
		PlayerInputBindingsInfo& operator=(PlayerInputBindingsInfo&&) noexcept = default;

		std::vector<std::unique_ptr<Command>> commands{};
		std::map<Command*, KeyboardBindingInfo> keyboardBindings{};
		std::map<Command*, ControllerBindingInfo> controllerBindings{};

		std::unique_ptr<Keyboard> pKeyboard{};
		std::unique_ptr<IController> pController{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput();

		//returns the next valid playerIdx
		size_t AddPlayer();
		InputManager& BindCommand(size_t playerIdx, std::unique_ptr<Command> command, KeyboardBindingInfo bindInfo);
		InputManager& BindCommand(size_t playerIdx, std::unique_ptr<Command> command, ControllerBindingInfo bindInfo);
	private:
		bool IsValidPlayerIdx(size_t playerIdx);

		std::vector<PlayerInputBindingsInfo> m_PlayerInputBindings{};
	};
}}
