#pragma once
#include "InputUtils.h"

namespace dae::Input
{
	class ControllerImpl
	{
	public:
		virtual ~ControllerImpl() = default;
		virtual void PollState(int controllerIdx = 0) = 0;
		virtual bool HasKeyState(unsigned int gamepadBUtton, KeyState keyState) const = 0;
		virtual bool IsDownThisFrame(unsigned int gamepadButton) const = 0;
		virtual bool IsUpThisFrame(unsigned int gamepadButton) const = 0;
		virtual bool IsPressed(unsigned int gamepadButton) const = 0;
		virtual unsigned int MapButton(unsigned int gamepadButton) const = 0;
	};
}