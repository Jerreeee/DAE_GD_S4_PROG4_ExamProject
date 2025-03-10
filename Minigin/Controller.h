#pragma once
#include "InputUtils.h"

namespace dae::Input
{
	class Controller
	{
	public:
		virtual ~Controller() = default;
		virtual void PollState(int controllerIdx = 0) = 0;
		virtual bool HasKeyState(uint32_t button, KeyState keyState) const = 0;
		virtual bool IsDownThisFrame(uint32_t button) const = 0;
		virtual bool IsUpThisFrame(uint32_t button) const = 0;
		virtual bool IsPressed(uint32_t button) const = 0;
	};
}