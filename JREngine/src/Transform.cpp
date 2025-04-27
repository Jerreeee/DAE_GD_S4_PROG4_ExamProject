#include "Transform.h"

const glm::vec3& JRE::Transform::GetPosition() const
{
	return m_Position;
}

void JRE::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void JRE::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}
