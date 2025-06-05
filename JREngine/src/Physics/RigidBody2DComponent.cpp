#include "Core/Timer.h"
#include "Scene/GameObject.h"
#include "Physics/RigidBody2DComponent.h"

namespace JRE
{
	RigidBody2DComponent::RigidBody2DComponent(GameObject& gameObject) :
		ComponentBase(gameObject)
	{
	}
	void RigidBody2DComponent::Update()
	{
		auto& obj = GetGameObject();

		m_Vel.y += m_Gravity * Timer::GetInstance().GetDeltaTime();

		glm::vec3 pos = obj.GetWorldPosition();
		const int pixPerM{ 20 };
		pos.x += m_Vel.x * pixPerM * Timer::GetInstance().GetDeltaTime();
		pos.y += m_Vel.y * pixPerM * Timer::GetInstance().GetDeltaTime();
		obj.SetWorldPosition(pos);
	}
	void RigidBody2DComponent::Launch(glm::vec2 dirAndStrength)
	{
		dirAndStrength.y *= -1;
		m_Vel = dirAndStrength;
	}
}
