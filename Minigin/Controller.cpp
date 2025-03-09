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

	bool Controller::HasKeyState(uint32_t button, KeyState keyState) const
	{
		return m_pImpl->HasKeyState(m_pImpl->MapButton(button), keyState);
	}

	bool Controller::IsDownThisFrame(uint32_t button) const
	{
		return m_pImpl->IsDownThisFrame(m_pImpl->MapButton(button));
	}

	bool Controller::IsUpThisFrame(uint32_t button) const
	{
		return m_pImpl->IsUpThisFrame(m_pImpl->MapButton(button));
	}

	bool Controller::IsPressed(uint32_t button) const
	{
		return m_pImpl->IsPressed(m_pImpl->MapButton(button));
	}
}
