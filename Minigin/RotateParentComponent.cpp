#include "RotateParentComponent.h"
#include <glm.hpp>
#include "GameObject.h"

namespace Engine
{
	RotateParentComponent::RotateParentComponent(GameObject& gameObject, float radius, float rotSpeedDeg)
		: ComponentBase(gameObject)
		, m_Radius{ radius }
		, m_RotSpeed{ glm::radians(rotSpeedDeg) }
	{
		Update();
	}

	void RotateParentComponent::Update()
	{
		m_Angle += m_RotSpeed * Timer::GetInstance().GetDeltaTime();
		float x = m_Radius * glm::cos(m_Angle);
		float y = m_Radius * glm::sin(m_Angle);
		GetGameObject().SetLocalPosition({x, y, 0});
	}
}
