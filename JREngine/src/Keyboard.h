#pragma once
#include <vector>
#include "InputUtils.h"

namespace JREngine::Input
{
	class Keyboard
	{
	public:
		virtual ~Keyboard() = default;
		virtual void AddKeysToTrack(const std::vector<KeyboardKey>& keys) = 0;
		virtual void PollState() = 0;
		virtual bool HasKeyState(KeyboardKey button, KeyState keyState) const = 0;
		virtual bool IsDownThisFrame(KeyboardKey key) const = 0;
		virtual bool IsUpThisFrame(KeyboardKey key) const = 0;
		virtual bool IsPressed(KeyboardKey key) const = 0;
	};
}