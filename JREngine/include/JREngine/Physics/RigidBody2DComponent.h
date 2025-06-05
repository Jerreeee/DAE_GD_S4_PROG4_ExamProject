#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Math/Rect.h"

namespace JRE
{
	class GameObject;
	class RigidBody2DComponent : public JRE::ComponentBase
	{
	public:
		RigidBody2DComponent(JRE::GameObject& gameObject);

		virtual void Update() override;

		void Launch(glm::vec2 dirAndStrength);
		inline static constexpr float m_Gravity{ 40.f };
	private:
		glm::vec2 m_Vel{};
	};
}
