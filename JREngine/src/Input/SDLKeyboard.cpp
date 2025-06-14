#include <SDL.h>
#include <algorithm>
#include <bitset>
#include "Input/SDLKeyboard.h"

namespace JRE::Input
{
	class SDLKeyboard::Impl final
	{
	public:
		void AddKeysToTrack(const std::vector<KeyboardKey>& keys);
		void RemoveKeysToTrack(const std::vector<KeyboardKey>& keys);
		void PollState();
		bool HasKeyState(KeyboardKey key, KeyState keyState) const;
		bool IsDownThisFrame(KeyboardKey key) const;
		bool IsUpThisFrame(KeyboardKey key) const;
		bool IsPressed(KeyboardKey key) const;
		uint32_t MapToInternalKey(KeyboardKey key) const;
	private:
		std::vector<KeyboardKey> m_TrackedKeys{};
		std::bitset<MAX_NR_KEYBOARD_KEYS> m_PreviousState{};
		std::bitset<MAX_NR_KEYBOARD_KEYS> m_CurrentState{};
		std::bitset<MAX_NR_KEYBOARD_KEYS> m_ButtonChanges{};
		std::bitset<MAX_NR_KEYBOARD_KEYS> m_ButtonsPressedThisFrame{};
		std::bitset<MAX_NR_KEYBOARD_KEYS> m_ButtonsReleasedThisFrame{};
	};
	void SDLKeyboard::Impl::AddKeysToTrack(const std::vector<KeyboardKey>& keys)
	{
		for (KeyboardKey key : keys)
			m_TrackedKeys.emplace_back(key);
	}

	void SDLKeyboard::Impl::RemoveKeysToTrack(const std::vector<KeyboardKey>& keys)
	{
		m_TrackedKeys.erase(std::remove_if(m_TrackedKeys.begin(), m_TrackedKeys.end(),
			[&](const KeyboardKey key)
			{
				return std::find(keys.begin(), keys.end(), key) != keys.end();
			}
		), m_TrackedKeys.end());
	}

	void SDLKeyboard::Impl::PollState()
	{
		m_PreviousState = m_CurrentState;

		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		for (int i{}; i < m_TrackedKeys.size(); ++i)
		{
			SDL_Scancode scancode = static_cast<SDL_Scancode>(MapToInternalKey(m_TrackedKeys[i]));
			m_CurrentState.set(static_cast<size_t>(m_TrackedKeys[i]), keyboardState[scancode]);
		}

		m_ButtonChanges = m_PreviousState ^ m_CurrentState;
		m_ButtonsPressedThisFrame = m_ButtonChanges & m_CurrentState;
		m_ButtonsReleasedThisFrame = m_ButtonChanges & (~m_CurrentState);
	}

	bool SDLKeyboard::Impl::HasKeyState(KeyboardKey key, KeyState keyState) const
	{
		switch (keyState)
		{
		case KeyState::Pressed:
			return IsPressed(key);
		case KeyState::DownThisFrame:
			return IsDownThisFrame(key);
		case KeyState::UpThisFrame:
			return IsUpThisFrame(key);
		case KeyState::Up:
			return true;
		default:
			return false;
		}
	}

	bool SDLKeyboard::Impl::IsDownThisFrame(KeyboardKey key) const
	{
		return m_ButtonsPressedThisFrame.test(static_cast<size_t>(key));
	}

	bool SDLKeyboard::Impl::IsUpThisFrame(KeyboardKey key) const
	{
		return m_ButtonsReleasedThisFrame.test(static_cast<size_t>(key));
	}

	bool SDLKeyboard::Impl::IsPressed(KeyboardKey key) const
	{
		return m_CurrentState.test(static_cast<size_t>(key));
	}

	uint32_t SDLKeyboard::Impl::MapToInternalKey(KeyboardKey key) const
	{
		uint32_t keyVal = static_cast<uint32_t>(key);
		if (keyVal <= 25)
			return keyVal + SDL_SCANCODE_A;
		else
			return keyVal - 26 + SDL_SCANCODE_F1;
	}

	JRE::Input::SDLKeyboard::SDLKeyboard() : m_pImpl{ std::make_unique<Impl>() } {};
	JRE::Input::SDLKeyboard::~SDLKeyboard() = default;
	void SDLKeyboard::AddKeysToTrack(const std::vector<KeyboardKey>& keysToTrack)
	{
		m_pImpl->AddKeysToTrack(keysToTrack);
	}
	void SDLKeyboard::PollState()
	{
		m_pImpl->PollState();
	}
	bool SDLKeyboard::HasKeyState(KeyboardKey key, KeyState keyState) const
	{
		return m_pImpl->HasKeyState(key, keyState);
	}
	bool SDLKeyboard::IsDownThisFrame(KeyboardKey key) const
	{
		return m_pImpl->IsDownThisFrame(key);
	}
	bool SDLKeyboard::IsUpThisFrame(KeyboardKey key) const
	{
		return m_pImpl->IsUpThisFrame(key);
	}
	bool SDLKeyboard::IsPressed(KeyboardKey key) const
	{
		return m_pImpl->IsPressed(key);
	}
}
