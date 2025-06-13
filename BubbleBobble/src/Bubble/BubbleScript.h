#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Observer.h"

namespace JRE
{
	class Box2DColliderComponent;
}

namespace BubbleBobble
{
	class ZenchanScriptComponent;
	class BubbleScript : public JRE::ComponentBase, public JRE::IObserver
	{
	public:
		BubbleScript(JRE::GameObject& gameObject, int direction);

		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		void Trap(ZenchanScriptComponent* enemy);
		void PopAndKill();
		void Burst();
		void SpawnFood();

		JRE::Box2DColliderComponent* m_pBox2ColliderCmp{ nullptr };

		int m_Direction{};
		glm::vec2 m_Vel{ 0.f, -50.f };
		float m_DecX{ 110.f };
		float m_SpeedX{ 150.0f };
		float m_AliveTime{};
		float m_MaxLifeTime{ 4.0f };
		ZenchanScriptComponent* m_TrappedEnemy{ nullptr };
	};
}
