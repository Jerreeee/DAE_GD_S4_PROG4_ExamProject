#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Command.h"

#include "Player/PlayerState.h"

namespace JRE
{
	class GameObject;
	class SpriteRendererComponent;
}

namespace BubbleBobble
{
	class PlayerScriptComponent : public JRE::ComponentBase
	{
	public:
		PlayerScriptComponent(JRE::GameObject& gameObject);

		virtual void Update() override;

		void Move(glm::vec2 direction);
	private:
		class MoveCommand final : public JRE::Command
		{
		public:
			MoveCommand(PlayerScriptComponent& player, glm::vec2 dir) : m_Player{ player }, m_Dir{dir} {}
			virtual void Execute() override
			{
				m_Player.m_Input.moveDir = m_Dir;
			};
		private:
			PlayerScriptComponent& m_Player;
			glm::vec2 m_Dir{};
		};

		JRE::GameObject& m_Player;
		JRE::SpriteRendererComponent* m_SpriteRendererComponent{};

		std::unique_ptr<IPlayerState> m_pState{};
		PlayerInput m_Input{};
		float m_Speed{ 30.f };
	};
}
