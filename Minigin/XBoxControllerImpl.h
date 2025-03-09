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
		virtual bool HasKeyState(uint32_t button, KeyState keyState) const override;
		virtual bool IsDownThisFrame(uint32_t button) const override;
		virtual bool IsUpThisFrame(uint32_t button) const override;
		virtual bool IsPressed(uint32_t button) const override;
		virtual uint32_t MapButton(uint32_t button) const override;
	private:
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		WORD m_ButtonChanges{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
	};
}
