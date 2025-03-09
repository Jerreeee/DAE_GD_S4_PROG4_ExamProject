#include "Controller.h"
#include "ControllerImpl.h"
#include "XBoxControllerImpl.h"

namespace dae::Input
{
	Controller::Controller()
	{
		m_pImpl = std::move(std::make_unique<XBoxControllerImpl>());
	}

	Controller::~Controller() {}

	void Controller::PollState(int controllerIdx)
	{
		m_pImpl->PollState(controllerIdx);
	}

	bool Controller::HasKeyState(unsigned int gamepadButton, KeyState keyState) const
	{
		return m_pImpl->HasKeyState(m_pImpl->MapButton(gamepadButton), keyState);
	}

	bool Controller::IsDownThisFrame(unsigned int gamepadButton) const
	{
		return m_pImpl->IsDownThisFrame(m_pImpl->MapButton(gamepadButton));
	}

	bool Controller::IsUpThisFrame(unsigned int gamepadButton) const
	{
		return m_pImpl->IsUpThisFrame(m_pImpl->MapButton(gamepadButton));
	}

	bool Controller::IsPressed(unsigned int gamepadButton) const
	{
		return m_pImpl->IsPressed(m_pImpl->MapButton(gamepadButton));
	}
}
