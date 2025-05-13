#pragma once
#include "JREngine/Core/Command.h"

namespace BubbleBobble::Player
{
	class ScriptComponent;
	class MoveCommand final : public JRE::Command
	{
	public:
		MoveCommand(ScriptComponent& player, int dir) : m_Player{ player }, m_Dir{ dir } {}
		virtual void Execute() override;
	private:
		ScriptComponent& m_Player;
		int m_Dir{};
	};
	class JumpCommand final : public JRE::Command
	{
	public:
		JumpCommand(ScriptComponent& player) : m_Player{ player } {}
		virtual void Execute() override;
	private:
		ScriptComponent& m_Player;
	};
	class ShootCommand final : public JRE::Command
	{
	public:
		ShootCommand(ScriptComponent& player) : m_Player{ player } {}
		virtual void Execute() override;
	private:
		ScriptComponent& m_Player;
	};
}
