#pragma once
#include <memory>
#include "InputUtils.h"

namespace dae::Input
{
	class ControllerImpl;
	class Controller
	{
	public:
		Controller();
		~Controller();

		void PollState(int controllerIdx = 0);

		bool HasKeyState(unsigned int gamepadButton, KeyState keyState) const;
		bool IsDownThisFrame(unsigned int gamepadButton) const;
		bool IsUpThisFrame(unsigned int gamepadButton) const;
		bool IsPressed(unsigned int gamepadButton) const;
	private:
		std::unique_ptr<ControllerImpl> m_pImpl{};
	};
}