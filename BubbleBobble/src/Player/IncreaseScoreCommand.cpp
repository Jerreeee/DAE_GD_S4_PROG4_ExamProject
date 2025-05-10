#include "Components/ScoreComponent.h"
#include "Player/IncreaseScoreCommand.h"

namespace BubbleBobble
{
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
