#include "SDL.h"
#undef main
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Physics/IPhysicsSystem.h"
#include "JREngine/Rendering/SDLRenderer.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

namespace JRE
{
	Box2DColliderComponent::Box2DColliderComponent(GameObject& gameObject)
		: RendererComponentBase(gameObject)
	{
	}
	void Box2DColliderComponent::OnEnable()
	{
		ServiceLocator::GetPhysicsSystem().RegisterCollider(this);
	}
	void Box2DColliderComponent::OnDisable()
	{
		ServiceLocator::GetPhysicsSystem().UnregisterCollider(this);
	}
	void Box2DColliderComponent::Render() const
	{
		//auto boxShpae = GetBounds();
		//SDLRenderer::GetInstance().DrawRectangle(static_cast<int>(boxShpae.offset.x),
		//	static_cast<int>(boxShpae.offset.y),
		//	static_cast<int>(boxShpae.width),
		//	static_cast<int>(boxShpae.height),
		//	SDL_Color{ 255, 0, 0, 255 });
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
