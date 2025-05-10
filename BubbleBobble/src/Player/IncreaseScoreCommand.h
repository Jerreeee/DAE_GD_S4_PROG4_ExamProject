#pragma once
#include "JREngine/Core/Command.h"

namespace BubbleBobble
{
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
