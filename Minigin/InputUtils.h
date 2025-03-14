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
	using ButtonState = KeyState;

	enum class KeyboardKey : uint32_t
	{
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
		Z
	};
	
	enum class ControllerButton
	{
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
	};
}
