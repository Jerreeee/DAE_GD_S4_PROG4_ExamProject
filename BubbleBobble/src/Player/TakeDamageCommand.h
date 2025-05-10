#pragma once
#include "JREngine/Core/Command.h"

namespace BubbleBobble
{
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
}
