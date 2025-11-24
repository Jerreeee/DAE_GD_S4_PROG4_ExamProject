#pragma once
#include <vector>
#include <map>
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Physics/BoxPhysicsSystem.h"
#include "JREngine/Core/Event.h"
#include "JREngine/Physics/Box2DColliderComponent.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Audio/ISoundClip.h"

#include "Components/HealthComponent.h"
#include "Player/PlayerUtils.h"

namespace JRE
{
	class GameObject;
	class SpriteAnimatorComponent;
	class SpriteRendererComponent;
	class Box2DColliderComponent;
	class SpriteAnimationClip;
}

namespace BubbleBobble
{
	namespace Events
	{
		struct PlayerLostLive
		{
			static const JRE::EventID ID{ JRE::HashEventID("PlayerLostLive") };
			struct Args : public JRE::EventArgs
			{
				Args(int _newHealth) : health{ _newHealth } {}
				int health;
			};
		};
	}

	class TileMapComponent;
	class PlayerScriptComponent final : public JRE::ComponentBase, public JRE::IObserver
	{
	public:
		PlayerScriptComponent(JRE::GameObject& gameObject);
		virtual ~PlayerScriptComponent() override;

		virtual void Update() override;
		virtual void FixedUpdate() override;

		virtual void OnNotify(JRE::EventInfo& event) override;

		void Move(int direction);
		void Jump();
		void Shoot();

		JRE::Event OnPlayerLostLive{};
		JRE::SoftAssetRef<JRE::ISoundClip> m_JumpSound{ nullptr };
		JRE::SoftAssetRef<JRE::ISoundClip> m_ShootSound{ nullptr };
	private:
		struct Input
		{
			bool pressedJump;
			int moveDir;
			bool pressedShoot;
		};

		enum class State
		{
			Mortal, Immortal, Stunned, Death
		};

		enum class AnimState
		{
			Moving, Shoot, Death
		};

		void MoveCollider();
		void ShootBubble();

		JRE::SpriteRendererComponent* m_pSpriteRendererCmp{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimatorCmp{ nullptr };
		JRE::Box2DColliderComponent* m_pBox2DColliderCmp{ nullptr };
		HealthComponent* m_pHealthCmp{ nullptr };
		JRE::AssetRef<JRE::SpriteAnimationClip> m_pShootClipRef{ nullptr };
		std::shared_ptr<JRE::EventConnection> m_ShootClipEndOfClipEventConn{ nullptr };
		JRE::AssetRef<JRE::SpriteAnimationClip> m_pDeathClipRef{ nullptr };
		std::shared_ptr<JRE::EventConnection> m_DeathClipEndOfClipEventConn{ nullptr };

		float m_ImmortalTimer{};
		float m_ImmortalTimerDefault{ 3.f };

		State m_State{ State::Mortal };
		AnimState m_AnimState{ AnimState::Moving };
		float m_Speed{ 15.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};
		int m_FacingDir{ 1 };
		JRE::CollisionInfo m_CollInfo{};
	};
}
