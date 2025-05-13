
#pragma once
#include <memory>
#include <array>
#include <string>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Core/Event.h"
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
		MovingState(ScriptComponent& player) : m_Player{ player } {};
		virtual void OnEnter() override;
		virtual State Update() override;
	private:
		ScriptComponent& m_Player;
	};

	class JumpState : public IState
	{
	public:
		JumpState(ScriptComponent& player) :m_Player{ player } {};
		virtual void OnEnter() override;
		virtual State Update() override;
	private:
		ScriptComponent& m_Player;
	};

	class ShootState : public IState, public JRE::IObserver
	{
	public:
		ShootState(ScriptComponent& player) : m_Player{ player } {};
		virtual void OnEnter() override;
		virtual State Update() override;

		void SetAnimationClip(JRE::AssetRef<JRE::SpriteAnimationClip> animClip);
		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		ScriptComponent& m_Player;
		JRE::AssetRef<JRE::SpriteAnimationClip> m_AnimClip{ nullptr };
		bool m_AnimClipEnded{ false };
	};

	class DiedState : public IState
	{
	public:
		DiedState(ScriptComponent& player) : m_Player{ player } {};
		virtual void OnEnter() override;
	private:
		ScriptComponent& m_Player;
	};
}
