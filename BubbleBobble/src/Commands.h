#pragma once
#include "glm.hpp"
#include "JREngine/Command.h"

namespace JREngine
{
	class GameObject;
}

namespace BubbleBobble
{
	class MoveCommand final : public JREngine::Command
	{
	public:
		MoveCommand(JREngine::GameObject& gameObject, float speed, glm::vec2 direction);

		virtual void Execute() override;
	private:
		JREngine::GameObject& m_GameObject;
		float m_Speed{};
		glm::vec2 m_Direction{};
	};

	class HealthComponent;
	class TakeDamageCommand final : public JREngine::Command
	{
	public:
		TakeDamageCommand(HealthComponent* pHealthComponent, int amount = 1);
		virtual void Execute() override;
	private:
		HealthComponent* m_pHealthComponent;
		int m_Amount;
	};

	class ScoreComponent;
	class IncreaseScoreCommand : public JREngine::Command
	{
	public:
		IncreaseScoreCommand(ScoreComponent* pScoreComponent, int amount = 1);
		virtual void Execute() override;
	private:
		ScoreComponent* m_pScoreComponent{};
		int m_Amount{};
	};
}