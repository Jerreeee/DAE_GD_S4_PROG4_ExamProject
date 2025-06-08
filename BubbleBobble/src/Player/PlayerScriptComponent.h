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
	class SpriteRendererComponent;
	class SpriteAnimatorComponent;
	class SpriteAnimationClip;
	class RigidBody2DComponent;
}

namespace BubbleBobble
{
	class TileMapComponent;
}

namespace BubbleBobble::Player
{
	class ScriptComponent : public JRE::ComponentBase
	{
	public:
		ScriptComponent(JRE::GameObject& gameObject, const JRE::Input::ActionMap& actionMap);

		virtual void Update() override;
		virtual void FixedUpdate() override;

		void SetActionMapToUse(const JRE::Input::ActionMap& actionMap);
		void SetAnimation(const std::string& animName, JRE::AssetRef<JRE::SpriteAnimationClip> clip);
	private:
		struct Input
		{
			bool pressedJump;
			bool movingLeft;
			bool movingRight;
		};

		void ChangeAnimation(Animation anim);

		JRE::GameObject& m_Player;
		const JRE::Input::ActionMap* m_pActionMap{ nullptr };
		JRE::SpriteAnimatorComponent* m_pSpriteAnimatiorComponent{ nullptr };
		TileMapComponent* m_pTileMapComponent{ nullptr };

		std::vector<JRE::AssetRef<JRE::SpriteAnimationClip>> m_Animations{};

		float m_Speed{ 20.f };
		float m_JumpForce{ 75.f };
		glm::vec2 m_Vel{};
		Input m_Input{};
		glm::vec2 m_ColliderOffset{ 4.f, 2.f };
		glm::vec2 m_ColliderSize{ 40.f, 46.f };
	};
}
