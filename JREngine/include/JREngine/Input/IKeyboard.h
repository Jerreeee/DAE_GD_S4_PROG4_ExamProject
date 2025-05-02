#pragma once
#include <vector>
#include "JREngine/Input/InputUtils.h"

namespace JRE::Input
{
	class IKeyboard
	{
	public:
		virtual ~IKeyboard() = default;
		virtual void AddKeysToTrack(const std::vector<KeyboardKey>& keys) = 0;
		virtual void PollState() = 0;
		virtual bool HasKeyState(KeyboardKey button, KeyState keyState) const = 0;
		virtual bool IsDownThisFrame(KeyboardKey key) const = 0;
		virtual bool IsUpThisFrame(KeyboardKey key) const = 0;
		virtual bool IsPressed(KeyboardKey key) const = 0;
	};
}