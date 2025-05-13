#pragma once
#include <vector>
#include <map>
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "Player/PlayerState.h"
#include "Player/PlayerCommands.h"

namespace JRE
{
	class GameObject;
	class SpriteRendererComponent;
	class SpriteAnimatorComponent;
	class SpriteAnimationClip;
	class RigidBody2DComponent;
}

namespace BubbleBobble::Player
{
	struct InitData
	{

	};

	class ScriptComponent : public JRE::ComponentBase
	{
	public:
		ScriptComponent(JRE::GameObject& gameObject);

		virtual void Start() override;
		virtual void Update() override;

		void SetAnimation(const std::string& animName, JRE::AssetRef<JRE::SpriteAnimationClip> clip);
	private:
		friend class MoveCommand;
		friend class JumpCommand;
		friend class ShootCommand;

		friend class MovingState;
		friend class JumpState;
		friend class ShootState;
		friend class DiedState;

		void Move(int direction);
		void Jump();
		void ChangeAnimation(Animation anim);

		JRE::GameObject& m_Player;
		JRE::SpriteAnimatorComponent* m_pSpriteAnimatiorComponent{ nullptr };
		JRE::RigidBody2DComponent* m_pRigidBody2DComponent{ nullptr };

		IState* m_pState{};
		std::vector<std::unique_ptr<IState>> m_States{};

		std::vector<JRE::AssetRef<JRE::SpriteAnimationClip>> m_Animations{};

		Input m_Input{};
		float m_Speed{ 30.f };
		float m_JumpForce{ 20.f };
	};
}
