#pragma once
#include <cassert>
#include "JREngine/Input/InputUtils.h"

namespace JRE::Input
{
	struct ControllerBindingInfo : public IBindingInfo
	{
		virtual DeviceType GetType() const { return GetStaticType(); };
		static DeviceType GetStaticType() { return DeviceType::Controller; };

		ControllerButton button;
		ButtonState buttonState;
	};

	class IController : public IInputDevice
	{
	public:
		virtual DeviceType GetType() const { return GetStaticType(); };
		static DeviceType GetStaticType() { return DeviceType::Controller; };

		virtual ~IController() = default;
		virtual void PollState(int controllerIdx = 0) = 0;
		virtual bool HasButtonState(ControllerButton button, ButtonState buttonState) const = 0;
		virtual bool IsDownThisFrame(ControllerButton button) const = 0;
		virtual bool IsUpThisFrame(ControllerButton button) const = 0;
		virtual bool IsPressed(ControllerButton button) const = 0;

		virtual bool IsBindingActive(const IBindingInfo& bindInfo) const override
		{
			assert(bindInfo.GetType() == IController::GetStaticType() && "bindInfo DeviceType mismatch");
			auto pBindInfo = static_cast<const ControllerBindingInfo*>(&bindInfo);
			return HasButtonState(pBindInfo->button, pBindInfo->buttonState);
		}
	};
}
