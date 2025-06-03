#pragma once
#include <memory>
#include "JREngine/Input/IController.h"

namespace JRE::Input
{
	class XBoxController final : public IController
	{
	public:
		XBoxController(int idx);
		~XBoxController();

		virtual void PollState() override;
		virtual bool HasButtonState(ControllerButton button, ButtonState buttonState) const override;
		virtual bool IsDownThisFrame(ControllerButton button) const override;
		virtual bool IsUpThisFrame(ControllerButton button) const override;
		virtual bool IsPressed(ControllerButton button) const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}
