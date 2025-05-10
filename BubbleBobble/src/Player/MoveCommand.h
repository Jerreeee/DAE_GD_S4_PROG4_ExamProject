#pragma once
#include "glm.hpp"
#include "JREngine/Core/Command.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class MoveCommand final : public JRE::Command
	{
	public:
		MoveCommand(JRE::GameObject& gameObject, float speed, glm::vec2 direction);

		virtual void Execute() override;
	private:
		JRE::GameObject& m_GameObject;
		float m_Speed{};
		glm::vec2 m_Direction{};
	};
}
