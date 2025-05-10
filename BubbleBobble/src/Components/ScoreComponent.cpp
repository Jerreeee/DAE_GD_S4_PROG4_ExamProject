#include "JREngine/Scene/GameObject.h"
#include "Events.h"
#include "Components/ScoreComponent.h"

namespace BubbleBobble
{
	ScoreComponent::ScoreComponent(JRE::GameObject& gameObject) :
		ComponentBase(gameObject),
		m_IncreasedScoreEvent{ std::make_unique<JRE::Observable>() }
	{
	}
	void ScoreComponent::IncreaseScore(int points)
	{
		m_Score += points;
		JRE::EventInfo e{ JRE::CreateEvent<Event::IncreasedScore>(m_Score) };
		m_IncreasedScoreEvent->NotifyObservers(e);
	}
}
