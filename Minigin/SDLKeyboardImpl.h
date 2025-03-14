#pragma once
#include <SDL.h>
#include <vector>
#include "KeyboardImpl.h"

namespace dae::Input
{
	class SDLKeyboardImpl final: public KeyboardImpl
	{
	public:
		virtual void AddKeysToTrack(const std::vector<KeyboardKey>& keysToTrack) override;
		virtual void PollState() override;
		virtual bool HasKeyState(uint32_t button, KeyState keyState) const override;
		virtual bool IsDownThisFrame(uint32_t button) const override;
		virtual bool IsUpThisFrame(uint32_t button) const override;
		virtual bool IsPressed(uint32_t button) const override;
		virtual uint32_t MapButton(uint32_t button) const override;
	private:
		std::vector<Button> m_TrackedKeys{};
		//bitset will be larger than necessary using this approach, could be optimized
		std::bitset<NR_KEYBOARD_BUTTONS> m_PreviousState{};
		std::bitset<NR_KEYBOARD_BUTTONS> m_CurrentState{};
		std::bitset<NR_KEYBOARD_BUTTONS> m_ButtonChanges{};
		std::bitset<NR_KEYBOARD_BUTTONS> m_ButtonsPressedThisFrame{};
		std::bitset<NR_KEYBOARD_BUTTONS> m_ButtonsReleasedThisFrame{};
	};
}