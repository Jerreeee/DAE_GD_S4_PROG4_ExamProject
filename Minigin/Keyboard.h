#pragma once
#include <memory>
#include <vector>
#include "KeyboardImpl.h"

namespace dae::Input
{
	class KeyboardImpl;
	class Keyboard final
	{
	public:
		Keyboard();
		~Keyboard();

		void AddKeysToTrack(const std::vector<KeyboardKey>& keysToTrack);
		void PollState();

		bool HasKeyState(uint32_t button, KeyState keyState) const;
		bool IsDownThisFrame(uint32_t button) const;
		bool IsUpThisFrame(uint32_t button) const;
		bool IsPressed(uint32_t button) const;
	private:
		std::unique_ptr<KeyboardImpl> m_pImpl{};
	};
}