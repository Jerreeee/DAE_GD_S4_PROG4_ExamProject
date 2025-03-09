#include "SDLKeyboardImpl.h"

void dae::Input::SDLKeyboardImpl::AddKeysToTrack(const std::vector<Button>& keysToTrack)
{
	for (Button key : keysToTrack)
		m_TrackedKeys.emplace_back(key);
}

void dae::Input::SDLKeyboardImpl::PollState()
{
	m_PreviousState = m_CurrentState;

	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	for (int i{}; i < m_TrackedKeys.size(); ++i)
		if (keyboardState[static_cast<SDL_Scancode>(MapButton(static_cast<uint32_t>(m_TrackedKeys[i])))])
			m_CurrentState.set(static_cast<uint32_t>(m_TrackedKeys[i]) - static_cast<uint32_t>(Button::A));

	m_ButtonChanges = m_PreviousState ^ m_CurrentState;
	m_ButtonsPressedThisFrame = m_ButtonChanges & m_CurrentState;
	m_ButtonsReleasedThisFrame = m_ButtonChanges & (~m_CurrentState);
}

bool dae::Input::SDLKeyboardImpl::HasKeyState(uint32_t button, KeyState keyState) const
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

bool dae::Input::SDLKeyboardImpl::IsDownThisFrame(uint32_t button) const
{
	return m_ButtonsPressedThisFrame.test(button);
}

bool dae::Input::SDLKeyboardImpl::IsUpThisFrame(uint32_t button) const
{
	return m_ButtonsReleasedThisFrame.test(button);
}

bool dae::Input::SDLKeyboardImpl::IsPressed(uint32_t button) const
{
	return m_CurrentState.test(button);
}

uint32_t dae::Input::SDLKeyboardImpl::MapButton(uint32_t button) const
{
	return button - static_cast<uint32_t>(Button::A) + static_cast<uint32_t>(SDL_SCANCODE_A); //map Button::A-Z -> SDL_SCANCODE_A-Z
}
