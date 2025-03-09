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

	enum class Button : uint32_t
	{
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		COUNT
	};

	//Could maybe be automated for easier use when new buttons are added
	constexpr size_t NR_GAMEPAD_BUTTONS = static_cast<size_t>(Button::DPAD_RIGHT) - static_cast<size_t>(Button::DPAD_UP) + 1;
	constexpr size_t NR_KEYBOARD_BUTTONS = static_cast<size_t>(Button::Z) - static_cast<size_t>(Button::A) + 1;

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
	inline const auto BUTTON_GAMEPAD_MASK = GetMask<static_cast<size_t>(Button::DPAD_UP), static_cast<size_t>(Button::DPAD_RIGHT)>();
	inline const auto BUTTON_KEYBOARD_MASK = GetMask<static_cast<size_t>(Button::A), static_cast<size_t>(Button::W)>();
}
