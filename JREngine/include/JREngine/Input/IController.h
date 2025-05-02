#pragma once
#include "JREngine/Input/InputUtils.h"

namespace JRE::Input
{
	class IController
	{
	public:
		virtual ~IController() = default;
		virtual void PollState(int controllerIdx = 0) = 0;
		virtual bool HasButtonState(ControllerButton button, ButtonState buttonState) const = 0;
		virtual bool IsDownThisFrame(ControllerButton button) const = 0;
		virtual bool IsUpThisFrame(ControllerButton button) const = 0;
		virtual bool IsPressed(ControllerButton button) const = 0;
	};
}