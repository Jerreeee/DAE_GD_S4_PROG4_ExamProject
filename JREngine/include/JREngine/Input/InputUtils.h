#pragma once
#include <type_traits>

namespace JRE::Input
{
	enum class DeviceType
	{
		Keyboard, Controller
	};

	struct IBindingInfo
	{
		virtual ~IBindingInfo() = default;
		virtual DeviceType GetType() const = 0;
	};

	class IInputDevice
	{
	public:
		virtual ~IInputDevice() = default;
		virtual DeviceType GetType() const = 0;
		virtual bool IsBindingActive(const IBindingInfo& bindInfo) const = 0;
	};

	enum class Direction
	{
		Up, Down, Left, Right
	};

	enum class KeyState
	{
		Up, DownThisFrame, UpThisFrame, Pressed
	};
	using ButtonState = KeyState;

	enum class KeyboardKey : uint8_t
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

	const auto MAX_NR_KEYBOARD_KEYS = 1 + static_cast<std::underlying_type_t<KeyboardKey>>(KeyboardKey::Z);
	
	enum class ControllerButton : uint8_t
	{
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		START,
		BACK,
		LEFT_THUMB,
		RIGHT_THUMB,
		LEFT_SHOULDER,
		RIGHT_SHOULDER,
		FACE_DOWN,
		FACE_RIGHT,
		FACE_LEFT,
		FACE_UP,
	};
}
