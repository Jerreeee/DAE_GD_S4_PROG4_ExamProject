#include "Transform.h"

const glm::vec3& JREngine::Transform::GetPosition() const
{
	return m_Position;
}

void JREngine::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void JREngine::Transform::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}
