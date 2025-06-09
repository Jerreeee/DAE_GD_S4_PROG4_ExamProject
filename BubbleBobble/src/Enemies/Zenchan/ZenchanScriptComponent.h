#pragma once
#include "JREngine/Scene/ComponentBase.h"

#include "TileMap/TileMapComponent.h"

namespace JRE
{
	class GameObject;
	class SpriteAnimatorComponent;
	class SpriteRendererComponent;
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

		const CollisionInfo& GetCollInfo() const { return m_CollInfo; };
	private:
		struct Input
		{
			bool pressedJump;
			int moveDir;
		};

		JRE::SpriteRendererComponent* m_pSpriteRenderer{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimator{ nullptr };
		TileMapComponent* m_pTileMapComponent{ nullptr };

		float m_Speed{ 20.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};
		CollisionInfo m_CollInfo{};
		int m_FacingDir{};

		glm::vec2 m_ColliderOffset{ 0.f, 0.f };
		glm::vec2 m_ColliderSize{ 48.f, 48.f };
	};
}
