#include "Components/HealthComponent.h"
#include "Player/TakeDamageCommand.h"

namespace BubbleBobble
{
	TakeDamageCommand::TakeDamageCommand(HealthComponent* pHealthComponent, int amount) :
		m_pHealthComponent{ pHealthComponent },
		m_Amount{ amount }
	{
	}
	void TakeDamageCommand::Execute()
	{
		m_pHealthComponent->TakeDamage(m_Amount);
	}
}
