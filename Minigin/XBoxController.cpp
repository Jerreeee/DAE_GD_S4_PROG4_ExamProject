#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#include "XBoxController.h"
#include <vector>

namespace Engine::Input
{

	class XBoxController::Impl final
	{
	public:
		void PollState(int controllerIdx);
		bool HasButtonState(ControllerButton button, ButtonState buttonState) const;
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button) const;
		bool IsPressed(ControllerButton button) const;
		uint32_t MapToInternalButton(ControllerButton button) const;
	private:
		static const std::vector<uint32_t> m_MapToInternalButtons;
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		WORD m_ButtonChanges{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
	};

	const std::vector<uint32_t> XBoxController::Impl::m_MapToInternalButtons = {
		XINPUT_GAMEPAD_DPAD_UP,
		XINPUT_GAMEPAD_DPAD_DOWN,
		XINPUT_GAMEPAD_DPAD_LEFT,
		XINPUT_GAMEPAD_DPAD_RIGHT,
		XINPUT_GAMEPAD_START,
		XINPUT_GAMEPAD_BACK,
		XINPUT_GAMEPAD_LEFT_THUMB,
		XINPUT_GAMEPAD_RIGHT_THUMB,
		XINPUT_GAMEPAD_LEFT_SHOULDER,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y
	};

	void XBoxController::Impl::PollState(int controllerIdx)
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
	bool XBoxController::Impl::HasButtonState(ControllerButton button, ButtonState buttonState) const
	{
		switch (buttonState)
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
	bool XBoxController::Impl::IsDownThisFrame(ControllerButton button) const
	{
		return m_ButtonsPressedThisFrame & MapToInternalButton(button);
	}
	bool XBoxController::Impl::IsUpThisFrame(ControllerButton button) const
	{
		return m_ButtonsReleasedThisFrame & MapToInternalButton(button);
	}
	bool XBoxController::Impl::IsPressed(ControllerButton button) const
	{
		return m_CurrentState.Gamepad.wButtons & MapToInternalButton(button);
	}
	uint32_t XBoxController::Impl::MapToInternalButton(ControllerButton button) const
	{
		return m_MapToInternalButtons[static_cast<uint32_t>(button)];
	}

	XBoxController::XBoxController() : m_pImpl{ std::make_unique<Impl>() } {}
	XBoxController::~XBoxController() {}
	void XBoxController::PollState(int controllerIdx)
	{
		m_pImpl->PollState(controllerIdx);
	}
	bool XBoxController::HasButtonState(ControllerButton button, ButtonState buttonState) const
	{
		return m_pImpl->HasButtonState(button, buttonState);
	}
	bool XBoxController::IsDownThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsDownThisFrame(button);
	}
	bool XBoxController::IsUpThisFrame(ControllerButton button) const
	{
		return m_pImpl->IsUpThisFrame(button);
	}
	bool XBoxController::IsPressed(ControllerButton button) const
	{
		return m_pImpl->IsPressed(button);
	}
}

