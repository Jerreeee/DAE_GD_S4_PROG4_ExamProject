#pragma once
#include <glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const;
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
	private:
		glm::vec3 m_Position;
	};
}
