#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Command.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class PlayerScriptComponent : public JRE::ComponentBase
	{
	public:
		PlayerScriptComponent(JRE::GameObject& gameObject);

		virtual void Update() override;
	private:
		float m_Speed{ 10.f };

		std::unique_ptr<JRE::Command> m_pMoveUpCommand;
		std::unique_ptr<JRE::Command> m_pMoveDownCommand;
		std::unique_ptr<JRE::Command> m_pMoveLeftCommand;
		std::unique_ptr<JRE::Command> m_pMoveRightCommand;
	};
}
