#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Physics/BoxPhysicsSystem.h"

#include "TileMap/TileMapComponent.h"

namespace JRE
{
	class GameObject;
	class SpriteAnimatorComponent;
	class SpriteRendererComponent;
	class Box2DColliderComponent;
}

namespace BubbleBobble
{
	class ZenchanScriptComponent final: public JRE::ComponentBase
	{
	public:
		ZenchanScriptComponent(JRE::GameObject& gameObject);

		virtual void Update() override {};
		void FixedUpdate() override;
		
		void Move(int direction);
		void Jump();

		const JRE::CollisionInfo& GetCollInfo() const { return m_CollInfo; };
	private:
		struct Input
		{
			bool pressedJump;
			int moveDir;
		};

		JRE::SpriteRendererComponent* m_pSpriteRendererCmp{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimatorCmp{ nullptr };
		JRE::Box2DColliderComponent* m_pBox2DColliderCmp{ nullptr };

		float m_Speed{ 10.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};
		JRE::CollisionInfo m_CollInfo{};
		int m_FacingDir{};
	};
}
