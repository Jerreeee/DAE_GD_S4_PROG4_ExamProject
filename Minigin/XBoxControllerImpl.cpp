#include "XBoxControllerImpl.h"
#include <winerror.h>
#include <winnt.h>

namespace dae::Input
{
	void XBoxControllerImpl::PollState(int controllerIdx)
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		DWORD result = XInputGetState(controllerIdx, &m_CurrentState);

		if (result == ERROR_SUCCESS)
		{
			m_ButtonChanges = m_PreviousState.Gamepad.wButtons ^ m_CurrentState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = m_ButtonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = m_ButtonChanges & (~m_CurrentState.Gamepad.wButtons);
		}
	}

	bool XBoxControllerImpl::HasKeyState(unsigned int gamepadButton, KeyState keyState) const
	{
		switch (keyState)
		{
		case KeyState::Pressed:
			return IsPressed(gamepadButton);
		case KeyState::DownThisFrame:
			return IsDownThisFrame(gamepadButton);
		case KeyState::UpThisFrame:
			return IsUpThisFrame(gamepadButton);
		case KeyState::Up:
			return true;
		default:
			return false;
		}
	}

	bool XBoxControllerImpl::IsDownThisFrame(unsigned int gamepadButton) const
	{
		return m_ButtonsPressedThisFrame & gamepadButton;
	}

	bool XBoxControllerImpl::IsUpThisFrame(unsigned int gamepadButton) const
	{
		return m_ButtonsReleasedThisFrame & gamepadButton;
	}

	bool XBoxControllerImpl::IsPressed(unsigned int gamepadButton) const
	{
		return m_CurrentState.Gamepad.wButtons & gamepadButton;
	}
	unsigned int XBoxControllerImpl::MapButton(unsigned int gamepadButton) const
	{
		return 1 << gamepadButton;
	}
}

