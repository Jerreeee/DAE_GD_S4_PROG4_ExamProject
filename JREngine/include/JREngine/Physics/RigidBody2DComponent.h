#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class GameObject;
	class RigidBody2DComponent : public JRE::ComponentBase
	{
	public:
		RigidBody2DComponent(JRE::GameObject& gameObject);

		virtual void Update() override;
	private:
		float m_Gravity{ 9.81f };
		//float m_Acc{};
		glm::vec2 m_Vel{};
	};
}
