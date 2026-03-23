#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Event.h"

namespace JRE
{
	class Box2DColliderComponent;
	class SpriteAnimatorComponent;
	class SpriteAnimationClip;
}

namespace BubbleBobble
{
	class ZenchanScriptComponent;
	class BubbleScript final : public JRE::ComponentBase
	{
	public:
		BubbleScript(JRE::GameObject& gameObject, int direction);

		virtual void Update() override;
		virtual void FixedUpdate() override;
	private:
		void OnCollision(JRE::EventInfo& event);
		void PopAndKill();
		void Burst();

		JRE::Box2DColliderComponent* m_pBox2ColliderCmp{ nullptr };
		std::shared_ptr<JRE::EventConnection> m_Box2DCollisionEventConn{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimCmp{ nullptr };
		JRE::AssetRef<JRE::SpriteAnimationClip> m_PoofAnimClip{ nullptr };
		std::shared_ptr<JRE::EventConnection> m_PoofAnimClipEndOfClipEventConn{ nullptr };

		bool m_Popped{ false };
		int m_Direction{};
		glm::vec2 m_Vel{ 0.f, -50.f };
		float m_DecX{ 110.f };
		float m_SpeedX{ 150.0f };
		float m_AliveTime{};
		float m_MaxLifeTime{ 4.0f };
		ZenchanScriptComponent* m_TrappedEnemy{ nullptr };
	};
}
