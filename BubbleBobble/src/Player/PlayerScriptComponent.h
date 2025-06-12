#pragma once
#include <vector>
#include <map>
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Physics/BoxPhysicsSystem.h"
#include "JREngine/Core/Event.h"

#include "Components/HealthComponent.h"
#include "Player/PlayerUtils.h"

namespace JRE
{
	class GameObject;
	class SpriteAnimatorComponent;
	class SpriteRendererComponent;
	class Box2DColliderComponent;
}

namespace BubbleBobble
{
	class TileMapComponent;
	class PlayerScriptComponent final : public JRE::ComponentBase, public JRE::IObserver
	{
	public:
		PlayerScriptComponent(JRE::GameObject& gameObject);

		virtual void Update() override {};
		virtual void FixedUpdate() override;

		virtual void OnNotify(JRE::EventInfo& event) override;

		void Move(int direction);
		void Jump();
	private:
		struct Input
		{
			bool pressedJump;
			int moveDir;
		};

		JRE::SpriteRendererComponent* m_pSpriteRendererCmp{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimatorCmp{ nullptr };
		JRE::Box2DColliderComponent* m_pBox2DColliderCmp{ nullptr };
		HealthComponent* m_pHealthCmp{ nullptr };

		float m_Speed{ 15.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};
		int m_FacingDir{};
	};
}
