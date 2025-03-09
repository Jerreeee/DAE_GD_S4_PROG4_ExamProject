#pragma once
#include <memory>
#include "InputUtils.h"

namespace dae::Input
{
	class ControllerImpl;
	class Controller final
	{
	public:
		Controller();
		~Controller();

		void PollState(int controllerIdx = 0);

		bool HasKeyState(uint32_t button, KeyState keyState) const;
		bool IsDownThisFrame(uint32_t button) const;
		bool IsUpThisFrame(uint32_t button) const;
		bool IsPressed(uint32_t button) const;
	private:
		std::unique_ptr<ControllerImpl> m_pImpl{};
	};
}