#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Physics/BoxPhysicsSystem.h"
#include "JREngine/Core/Event.h"

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
	namespace Events
	{
		struct EnemyDied
		{
			static const JRE::EventID ID{ JRE::HashEventID("EnemyDied") };
			struct Args : public JRE::EventArgs
			{
				Args(int _points) : points{ _points } {}
				int points;
			};
		};
	}

	class ZenchanScriptComponent final: public JRE::ComponentBase
	{
	public:
		ZenchanScriptComponent(JRE::GameObject& gameObject);
		virtual ~ZenchanScriptComponent() override;

		virtual void Update() override;
		void FixedUpdate() override;
		
		void Move(int direction);
		void Jump();

		void Kill();
		void SetTrappedBy(JRE::GameObject* bubble);
		bool IsTrapped() const { return m_State == State::Trapped; }

		const JRE::CollisionInfo& GetCollInfo() const { return m_CollInfo; };

		JRE::Event OnEnemyDied{};
	private:
		struct Input
		{
			bool pressedJump;
			int moveDir;
		};

		enum class State
		{
			Normal, Trapped, Angry
		};

		JRE::SpriteRendererComponent* m_pSpriteRendererCmp{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimatorCmp{ nullptr };
		JRE::Box2DColliderComponent* m_pBox2DColliderCmp{ nullptr };

		State m_State{ State::Normal };
		float m_Speed{ 10.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};
		JRE::CollisionInfo m_CollInfo{};
		int m_FacingDir{};

		JRE::GameObject* m_pBubble{ nullptr };
	};
}
