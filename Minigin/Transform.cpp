#include "Transform.h"

const glm::vec3& dae::Transform::GetPosition() const
{
	return m_Position;
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}
