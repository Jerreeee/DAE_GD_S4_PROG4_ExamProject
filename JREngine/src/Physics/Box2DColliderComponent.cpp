#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Physics/IPhysicsSystem.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

namespace JRE
{
	Box2DColliderComponent::Box2DColliderComponent(GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		ServiceLocator::GetPhysicsSystem().RegisterCollider(this);
	}
	Box2DColliderComponent::~Box2DColliderComponent()
	{
		ServiceLocator::GetPhysicsSystem().UnregisterCollider(this);
	}
	BoxShape Box2DColliderComponent::GetBounds() const
	{
		glm::vec2 worldPos = glm::vec2(GetGameObject().GetWorldPosition());
		return m_Shape.Translated(worldPos);
	}
	void Box2DColliderComponent::OnCollisionWith(const ICollider& other)
	{
		EventInfo e{ CreateEvent<Events::Box2DCollisionEvent>(*this, other) };
		OnCollisionEvent.Notify(e);
	}
}
