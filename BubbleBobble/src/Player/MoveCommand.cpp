#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/Timer.h"
#include "Player/MoveCommand.h"

namespace BubbleBobble
{
	MoveCommand::MoveCommand(JRE::GameObject& gameObject, float speed, glm::vec2 direction) :
		m_GameObject{ gameObject },
		m_Speed{ speed },
		m_Direction{ direction }
	{
	}

	void MoveCommand::Execute()
	{
		glm::vec3 pos = m_GameObject.GetLocalPosition();
		float offset = m_Speed * JRE::Timer::GetInstance().GetDeltaTime();
		pos.x += offset * m_Direction.x;
		pos.y += offset * m_Direction.y;
		m_GameObject.SetLocalPosition(pos);
	}
}
