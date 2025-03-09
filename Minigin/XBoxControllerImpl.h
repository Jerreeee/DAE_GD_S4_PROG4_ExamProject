#pragma once
#include "ControllerImpl.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#include <map>
#include "Controller.h"

namespace dae::Input
{
	class XBoxControllerImpl final : public ControllerImpl
	{
	public:
		virtual void PollState(int controllerIdx = 0) override;
		virtual bool HasKeyState(unsigned int gamepadButton, KeyState keyState) const override;
		virtual bool IsDownThisFrame(unsigned int gamepadButton) const override;
		virtual bool IsUpThisFrame(unsigned int gamepadButton) const override;
		virtual bool IsPressed(unsigned int gamepadButton) const override;
		virtual unsigned int MapButton(unsigned int gamepadButton) const override;
	private:
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		WORD m_ButtonChanges{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
	};
}
