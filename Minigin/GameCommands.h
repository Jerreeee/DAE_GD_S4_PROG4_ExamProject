#pragma once
#include "Command.h"
#include "glm.hpp"

namespace dae
{
	//Temporarily in here, should be in game files
	class GameObject;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject& gameObject, float speed, glm::vec2 direction);

		virtual void Execute() override;
	private:
		GameObject& m_GameObject;
		float m_Speed{};
		glm::vec2 m_Direction{};
	};
}