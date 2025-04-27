#pragma once
#include "glm.hpp"
#include "JREngine/Command.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class MoveCommand final : public JRE::Command
	{
	public:
		MoveCommand(JRE::GameObject& gameObject, float speed, glm::vec2 direction);

		virtual void Execute() override;
	private:
		JRE::GameObject& m_GameObject;
		float m_Speed{};
		glm::vec2 m_Direction{};
	};

	class HealthComponent;
	class TakeDamageCommand final : public JRE::Command
	{
	public:
		TakeDamageCommand(HealthComponent* pHealthComponent, int amount = 1);
		virtual void Execute() override;
	private:
		HealthComponent* m_pHealthComponent;
		int m_Amount;
	};

	class ScoreComponent;
	class IncreaseScoreCommand : public JRE::Command
	{
	public:
		IncreaseScoreCommand(ScoreComponent* pScoreComponent, int amount = 1);
		virtual void Execute() override;
	private:
		ScoreComponent* m_pScoreComponent{};
		int m_Amount{};
	};
}