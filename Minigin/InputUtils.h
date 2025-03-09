#pragma once
#include <bitset>
#include <iostream>

namespace dae::Input
{
	class Keyboard;
	class Controller;

	enum class Direction
	{
		Up, Down, Left, Right
	};

	enum class KeyState
	{
		Up, DownThisFrame, UpThisFrame, Pressed
	};

	enum class Button
	{
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,

		A,
		D,
		S,
		W,

		COUNT
	};

	//Create bitset mask, (bitset only constexpr since c++23)
	template<size_t START, size_t END>
	std::bitset<static_cast<size_t>(END - START + 1)> GetMask()
	{
		std::bitset<static_cast<size_t>(END - START + 1)> mask{};
		for (size_t i = START; i < END + 1; ++i)
			mask.set(i - START);
		return mask;
	};

	//created before main()
	//Maybe could be automated more so adding a button doesn't require to change anything here
	inline auto BUTTON_GAMEPAD_MASK = GetMask<static_cast<size_t>(Button::DPAD_UP), static_cast<size_t>(Button::DPAD_RIGHT)>();
	inline auto BUTTON_KEYBOARD_MASK = GetMask<static_cast<size_t>(Button::A), static_cast<size_t>(Button::W)>();
}
