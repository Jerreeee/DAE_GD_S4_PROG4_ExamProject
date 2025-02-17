#pragma once
#include <glm.hpp>
#include "ComponentBase.h"

namespace dae
{
	class TransformComponent final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position;
	};
}
