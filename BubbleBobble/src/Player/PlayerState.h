#pragma once
#include <memory>

namespace BubbleBobble
{
	class PlayerScriptComponent;

	struct PlayerInput
	{
		glm::vec2 moveDir;
		bool jumping;
	};

	class IPlayerState
	{
	public:
		virtual ~IPlayerState() = default;
		virtual void OnEnter() {};
		virtual void OnHandleInput(const PlayerInput&) {};
		virtual std::unique_ptr<IPlayerState> Update() { return nullptr; };
		virtual void OnExit() {};
	};

	class PlayerIdleState : public IPlayerState
	{
	public:
		PlayerIdleState(PlayerScriptComponent& player) : m_Player{ player } {};
		virtual void OnEnter() override;
		virtual void OnHandleInput(const PlayerInput& input) override;
		virtual std::unique_ptr<IPlayerState> Update() override;
	private:
		PlayerScriptComponent& m_Player;
		bool m_Moving{};
		float m_Eps{ 0.01f };
	};

	class PlayerRunningState : public IPlayerState
	{
	public:
		PlayerRunningState(PlayerScriptComponent& player) : m_Player{ player } {};
		virtual void OnEnter() override;
		virtual void OnHandleInput(const PlayerInput& input) override;
		virtual std::unique_ptr<IPlayerState> Update() override;
	private:
		PlayerScriptComponent& m_Player;
		bool m_Moving{};
		float m_Eps{ 0.01f };
	};

	class PlayerDiedState : public IPlayerState
	{
	public:
		virtual void OnEnter() override;
	};
}
