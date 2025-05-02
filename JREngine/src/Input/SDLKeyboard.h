#pragma once
#include <memory>
#include "JREngine/Input/IKeyboard.h"

namespace JRE::Input
{
	class SDLKeyboard final: public IKeyboard
	{
	public:
		SDLKeyboard();
		~SDLKeyboard();

		virtual void AddKeysToTrack(const std::vector<KeyboardKey>& keys) override;
		virtual void PollState() override;
		virtual bool HasKeyState(KeyboardKey key, KeyState keyState) const override;
		virtual bool IsDownThisFrame(KeyboardKey key) const override;
		virtual bool IsUpThisFrame(KeyboardKey key) const override;
		virtual bool IsPressed(KeyboardKey key) const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}