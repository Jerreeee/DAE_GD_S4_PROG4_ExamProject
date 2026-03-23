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

		m_Box2DCollisionEventConn = m_pBox2DColliderCmp->OnCollisionEvent.AddObserver(
			[this](JRE::EventInfo& e)
			{
				auto& args = e.GetArgs<JRE::Events::Box2DCollisionEvent>();
				if (args.other.GetProperties().layer & CollisionMask::DynamicGameObject)
				{
					auto& go = args.other.GetOwner();
					glm::vec2 pos = glm::vec2(go.GetWorldPosition());
					go.SetWorldPosition(pos.x, 0.f);
				}
			});
	}
}
