#include "JREngine/Scene/GameObject.h"
#include "Events.h"
#include "Components/ScoreComponent.h"

namespace BubbleBobble
{
	ScoreComponent::ScoreComponent(JRE::GameObject& gameObject) :
		ComponentBase(gameObject)
	{
	}
	void ScoreComponent::IncreaseScore(int points)
	{
		m_Score += points;
		JRE::EventInfo e{ JRE::CreateEvent<Event::IncreasedScore>(m_Score) };
		OnIncreasedScoreEvent.Notify(e);
	}
}
