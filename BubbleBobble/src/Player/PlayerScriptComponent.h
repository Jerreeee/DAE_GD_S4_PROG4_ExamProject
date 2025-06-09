#pragma once
#include <vector>
#include <map>
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "Player/PlayerUtils.h"

namespace JRE
{
	class GameObject;
	class SpriteAnimatorComponent;
}

namespace BubbleBobble
{
	class TileMapComponent;
	class PlayerScriptComponent : public JRE::ComponentBase
	{
	public:
		PlayerScriptComponent(JRE::GameObject& gameObject);

		virtual void Update() override;
		virtual void FixedUpdate() override;

		void Move(int direction);
		void Jump();
	private:
		struct Input
		{
			bool pressedJump;
			int moveDir;
		};

		JRE::SpriteAnimatorComponent* m_pSpriteAnimator{ nullptr };
		TileMapComponent* m_pTileMapComponent{ nullptr };

		float m_Speed{ 20.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};

		glm::vec2 m_ColliderOffset{ 4.f, 2.f };
		glm::vec2 m_ColliderSize{ 40.f, 46.f };
	};
}
