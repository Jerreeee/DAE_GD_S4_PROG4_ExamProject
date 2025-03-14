#include "Keyboard.h"
#include "SDLKeyboardImpl.h"

dae::Input::Keyboard::Keyboard()
{
	m_pImpl = std::make_unique<SDLKeyboardImpl>();
}

dae::Input::Keyboard::~Keyboard() {}

void dae::Input::Keyboard::AddKeysToTrack(const std::vector<KeyboardKey>& keysToTrack)
{
	m_pImpl->AddKeysToTrack(keysToTrack);
}

void dae::Input::Keyboard::PollState()
{
	m_pImpl->PollState();
}

bool dae::Input::Keyboard::HasKeyState(uint32_t button, KeyState keyState) const
{
	return m_pImpl->HasKeyState(m_pImpl->MapButton(button), keyState);
}

bool dae::Input::Keyboard::IsDownThisFrame(uint32_t button) const
{
	return m_pImpl->IsDownThisFrame(m_pImpl->MapButton(button));
}

bool dae::Input::Keyboard::IsUpThisFrame(uint32_t button) const
{
	return m_pImpl->IsUpThisFrame(m_pImpl->MapButton(button));
}

bool dae::Input::Keyboard::IsPressed(uint32_t button) const
{
	return m_pImpl->IsPressed(m_pImpl->MapButton(button));
}
