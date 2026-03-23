#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Event.h"

namespace JRE
{
	class Box2DColliderComponent;
}

namespace BubbleBobble
{
	class PortalScriptComponent final : public JRE::ComponentBase
	{
	public:
		PortalScriptComponent(JRE::GameObject& gameObject);

		virtual void Update() override {};
	private:
		JRE::Box2DColliderComponent* m_pBox2DColliderCmp{ nullptr };
		std::shared_ptr<JRE::EventConnection> m_Box2DCollisionEventConn{ nullptr };
	};
}
