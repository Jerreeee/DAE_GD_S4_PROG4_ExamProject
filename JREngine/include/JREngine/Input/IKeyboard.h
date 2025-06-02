#pragma once
#include <vector>
#include <cassert>
#include "JREngine/Input/InputUtils.h"

namespace JRE::Input
{
	struct KeyboardBindingInfo : public IBindingInfo
	{
		virtual DeviceType GetType() const { return GetStaticType(); };
		static DeviceType GetStaticType() { return DeviceType::Keyboard; };

		KeyboardKey key;
		KeyState keyState;
	};

	class IKeyboard : public IInputDevice
	{
	public:
		virtual DeviceType GetType() const { return GetStaticType(); };
		static DeviceType GetStaticType() { return DeviceType::Keyboard; };

		virtual ~IKeyboard() = default;
		virtual void AddKeysToTrack(const std::vector<KeyboardKey>& keys) = 0;
		virtual void PollState() = 0;
		virtual bool HasKeyState(KeyboardKey button, KeyState keyState) const = 0;
		virtual bool IsDownThisFrame(KeyboardKey key) const = 0;
		virtual bool IsUpThisFrame(KeyboardKey key) const = 0;
		virtual bool IsPressed(KeyboardKey key) const = 0;

		virtual bool IsBindingActive(const IBindingInfo& bindInfo) const override
		{
			assert(bindInfo.GetType() == IKeyboard::GetStaticType() && "bindInfo DeviceType mismatch");
			auto pBindInfo = static_cast<const KeyboardBindingInfo*>(&bindInfo);
			return HasKeyState(pBindInfo->key, pBindInfo->keyState);
		}
	};
}
