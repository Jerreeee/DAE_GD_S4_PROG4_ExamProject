#pragma once
#include <memory>
#include "Controller.h"

namespace dae::Input
{
	class XBoxController final : public Controller
	{
	public:
		XBoxController();
		~XBoxController();

		virtual void PollState(int controllerIdx = 0) override;
		virtual bool HasKeyState(uint32_t button, KeyState keyState) const override;
		virtual bool IsDownThisFrame(uint32_t button) const override;
		virtual bool IsUpThisFrame(uint32_t button) const override;
		virtual bool IsPressed(uint32_t button) const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}
