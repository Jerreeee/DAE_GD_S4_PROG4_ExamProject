#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Observer.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class ScoreComponent final : public JRE::ComponentBase
	{
	public:
		ScoreComponent(JRE::GameObject& gameObject);
		void IncreaseScore(int points);
		int GetScore() const { return m_Score; };

		virtual void Update() override {};
		JRE::Observable& OnIncreasedScoreEvent() { return *(m_IncreasedScoreEvent.get()); };
	private:
		int m_Score{};
		JRE::Event_t m_IncreasedScoreEvent{};
	};
}
