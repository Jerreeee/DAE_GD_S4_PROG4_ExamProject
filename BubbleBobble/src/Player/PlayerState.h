
#pragma once
#include <memory>
#include <array>
#include <string>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Core/Event.h"
#include "JREngine/Input/InputManager.h"
#include "Player/PlayerUtils.h"

namespace JRE
{
	class SpriteAnimationClip;
}

namespace BubbleBobble::Player
{
	class ScriptComponent;
	enum class State
	{
		Moving, Shooting, Jumping, Died, None
	};
	inline static constexpr std::array<std::string_view, 5> s_StateNames{
		"Moving", "Shooting", "Jumping", "Died", "None"
	};

	class IState
	{
	public:
		virtual ~IState() = default;
		virtual void OnEnter() {};
		virtual State Update() { return State::None; };
		virtual void OnExit() {};
	};

	class MovingState : public IState
	{
	public:
		MovingState(ScriptComponent& player, const JRE::Input::ActionMap*& pActionMap)
			: m_Player{ player }
			, m_pActionMap{ pActionMap } {};
		virtual void OnEnter() override;
		virtual State Update() override;
	private:
		ScriptComponent& m_Player;
		const JRE::Input::ActionMap*& m_pActionMap;
	};

	class JumpState : public IState
	{
	public:
		JumpState(ScriptComponent& player, const JRE::Input::ActionMap*& pActionMap, float force)
			: m_Player{ player }
			, m_pActionMap{ pActionMap }
			, m_Force{ force }
		{};
		virtual void OnEnter() override;
		virtual State Update() override { return State::Moving; };
	private:
		ScriptComponent& m_Player;
		const JRE::Input::ActionMap*& m_pActionMap;
		float m_Force{ 20.f };
	};

	class ShootState : public IState
	{
	public:
		ShootState(ScriptComponent& player, const JRE::Input::ActionMap*& pActionMap) : m_Player{ player }, m_pActionMap{ pActionMap } {};
		virtual void OnEnter() override;
		virtual State Update() override { return State::Moving; };
	private:
		ScriptComponent& m_Player;
		const JRE::Input::ActionMap*& m_pActionMap;
	};

	class DiedState : public IState
	{
	public:
		DiedState(ScriptComponent& player, const JRE::Input::ActionMap*& pActionMap) : m_Player{ player }, m_pActionMap{ pActionMap } {};
		virtual void OnEnter() override;
	private:
		ScriptComponent& m_Player;
		const JRE::Input::ActionMap*& m_pActionMap;
	};
}
