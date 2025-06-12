#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Observer.h"
#include "JREngine/Core/Event.h"
#include "JREngine/Physics/ICollider.h"

namespace JRE
{
	class Box2DColliderComponent;
	struct Box2DCollisionEvent
	{
		static constexpr EventID ID = HashEventID("Box2DCollision");

		struct Args : EventArgs
		{
			Args(const Box2DColliderComponent& _box2d, const ICollider& _other) : box2D{ _box2d }, other { _other } {}
			const Box2DColliderComponent& box2D;
			const ICollider& other;
		};
	};

	class Box2DColliderComponent final : public ComponentBase, public ICollider
	{
	public:
		Box2DColliderComponent(GameObject& gameObject);
		~Box2DColliderComponent();

		virtual void Update() override {};

		virtual const GameObject& GetOwner() const { return GetGameObject(); };
		virtual const ICollisionShape& GetShape() const override { return m_Shape; };
		virtual BoxShape GetBounds() const override;
		virtual const ColliderProperties& GetProperties() const override { return m_Properties; };
		virtual void OnCollisionWith(const ICollider& other) override;

		Event OnCollisionEvent{};

		BoxShape m_Shape{};
		ColliderProperties m_Properties{};
	};
}
