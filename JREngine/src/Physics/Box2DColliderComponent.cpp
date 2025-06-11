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
	void Box2DColliderComponent::OnCollisionWith(const ICollider& other)
	{
		EventInfo e{ CreateEvent<Box2DCollisionEvent>(*this, other) };
		OnCollisionEvent.Notify(e);
	}
}
