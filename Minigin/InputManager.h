#pragma once
#include <vector>
#include "Singleton.h"
#include "InputUtils.h"
#include <map>
#include <memory>
#include "Controller.h"
#include "Command.h"

namespace dae::Input
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		bool ProcessInput();

		void BindCommand(Button button, std::unique_ptr<Command> command, KeyState keyState);
	private:
		std::unique_ptr<Controller> m_pController{};
		std::map<Button, Command::Binding> m_ButtonCommandMap{};
	};
}
