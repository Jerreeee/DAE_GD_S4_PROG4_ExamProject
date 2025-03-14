#pragma once
#include "InputUtils.h"

namespace dae::Input
{
	class KeyboardImpl
	{
	public:
		virtual ~KeyboardImpl() = default;
		virtual void AddKeysToTrack(const std::vector<KeyboardKey>& keysToTrack) = 0;
		virtual void PollState() = 0;
		virtual bool HasKeyState(uint32_t button, KeyState keyState) const = 0;
		virtual bool IsDownThisFrame(uint32_t button) const = 0;
		virtual bool IsUpThisFrame(uint32_t button) const = 0;
		virtual bool IsPressed(uint32_t button) const = 0;
		virtual uint32_t MapButton(uint32_t button) const = 0;
	};
}