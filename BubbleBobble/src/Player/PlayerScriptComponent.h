#pragma once
#include <vector>
#include <map>
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "Player/PlayerState.h"

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
	class ScriptComponent : public JRE::ComponentBase
	{
	public:
		ScriptComponent(JRE::GameObject& gameObject, const JRE::Input::ActionMap& actionMap);

		virtual void Start() override;
		virtual void Update() override;

		void SetActionMapToUse(const JRE::Input::ActionMap& actionMap);
		void SetAnimation(const std::string& animName, JRE::AssetRef<JRE::SpriteAnimationClip> clip);

		void Move(int direction);
		void Jump(float force);
		void ChangeAnimation(Animation anim);
	private:
		JRE::GameObject& m_Player;

		JRE::SpriteAnimatorComponent* m_pSpriteAnimatiorComponent{ nullptr };
		JRE::RigidBody2DComponent* m_pRigidBody2DComponent{ nullptr };

		const JRE::Input::ActionMap* m_pActionMap{ nullptr };
		int m_ActionMapIdx{ -1 };
		IState* m_pState{};
		std::vector<std::unique_ptr<IState>> m_States{};

		std::vector<JRE::AssetRef<JRE::SpriteAnimationClip>> m_Animations{};
		float m_Speed{ 30.f };
	};
}
