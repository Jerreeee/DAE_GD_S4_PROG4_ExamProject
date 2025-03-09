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

	bool XBoxControllerImpl::HasKeyState(uint32_t button, KeyState keyState) const
	{
		switch (keyState)
		{
		case KeyState::Pressed:
			return IsPressed(button);
		case KeyState::DownThisFrame:
			return IsDownThisFrame(button);
		case KeyState::UpThisFrame:
			return IsUpThisFrame(button);
		case KeyState::Up:
			return true;
		default:
			return false;
		}
	}

	bool XBoxControllerImpl::IsDownThisFrame(uint32_t button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}

	bool XBoxControllerImpl::IsUpThisFrame(uint32_t button) const
	{
		return m_ButtonsReleasedThisFrame & button;
	}

	bool XBoxControllerImpl::IsPressed(uint32_t button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	}
	uint32_t XBoxControllerImpl::MapButton(uint32_t button) const
	{
		return 1 << button;
	}
}

