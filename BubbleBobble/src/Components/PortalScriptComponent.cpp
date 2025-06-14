#include "JREngine/Scene/GameObject.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

#include "EngineSetup.h"
#include "Components/PortalScriptComponent.h"

using namespace JRE;

namespace BubbleBobble
{
	PortalScriptComponent::PortalScriptComponent(JRE::GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		m_pBox2DColliderCmp = GetGameObject().GetComponent<Box2DColliderComponent>();
		assert(m_pBox2DColliderCmp && "GameObject containg PortalScriptComponent must also have Box2DColliderComponent");

		m_pBox2DColliderCmp->OnCollisionEvent.AddObserver(this);
	}
	PortalScriptComponent::~PortalScriptComponent()
	{
		m_pBox2DColliderCmp->OnCollisionEvent.RemoveObserver(this);
	}
	void PortalScriptComponent::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case JRE::Events::EventDestroyed::ID:
		{
			auto& args = event.GetArgs<JRE::Events::EventDestroyed>();
			args.event.RemoveObserver(this);
			break;
		}
		case JRE::Events::Box2DCollisionEvent::ID:
		{
			auto& args = event.GetArgs<JRE::Events::Box2DCollisionEvent>();
			if (args.other.GetProperties().layer & CollisionMask::DynamicGameObject)
			{
				auto& go = args.other.GetOwner();
				glm::vec2 pos = glm::vec2(go.GetWorldPosition());
				go.SetWorldPosition(pos.x, 0.f);
			}
			break;
		}
		}
	}
}
