#include <glm.hpp>
#include "JREngine/GameObject.h"
#include "RotateParentComponent.h"

namespace BubbleBobble
{
	RotateParentComponent::RotateParentComponent(JREngine::GameObject& gameObject, float radius, float rotSpeedDeg)
		: ComponentBase(gameObject)
		, m_Radius{ radius }
		, m_RotSpeed{ glm::radians(rotSpeedDeg) }
	{
		Update();
	}

	void RotateParentComponent::Update()
	{
		m_Angle += m_RotSpeed * JREngine::Timer::GetInstance().GetDeltaTime();
		float x = m_Radius * glm::cos(m_Angle);
		float y = m_Radius * glm::sin(m_Angle);
		GetGameObject().SetLocalPosition({x, y, 0});
	}
}
