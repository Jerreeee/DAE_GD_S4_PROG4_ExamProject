#include "JREngine/GameObject.h"
#include "JREngine/Timer.h"

#include "Commands.h"
#include "Components.h"

namespace BubbleBobble
{
	MoveCommand::MoveCommand(JRE::GameObject& gameObject, float speed, glm::vec2 direction) :
		m_GameObject{ gameObject },
		m_Speed{ speed },
		m_Direction{ direction }
	{
	}

	void MoveCommand::Execute()
	{
		glm::vec3 pos = m_GameObject.GetLocalPosition();
		float offset = m_Speed * JRE::Timer::GetInstance().GetDeltaTime();
		pos.x += offset * m_Direction.x;
		pos.y += offset * m_Direction.y;
		m_GameObject.SetLocalPosition(pos);
	}
	TakeDamageCommand::TakeDamageCommand(HealthComponent* pHealthComponent, int amount) :
		m_pHealthComponent{ pHealthComponent },
		m_Amount{ amount }
	{
	}
	void TakeDamageCommand::Execute()
	{
		m_pHealthComponent->TakeDamage(m_Amount);
	}
	IncreaseScoreCommand::IncreaseScoreCommand(ScoreComponent* pScoreComponent, int amount) :
		m_pScoreComponent{ pScoreComponent },
		m_Amount{ amount }
	{
	}
	void IncreaseScoreCommand::Execute()
	{
		m_pScoreComponent->IncreaseScore(m_Amount);
	}
}
