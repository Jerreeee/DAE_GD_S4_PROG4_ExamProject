#include "Command.h"
#include "glm.hpp"
#include "Timer.h"

dae::GameActorCommand::GameActorCommand(GameObject& gameObject) :
	m_GameObject{ gameObject }
{
}

dae::GameObject& dae::GameActorCommand::GetGameActor() const
{
	return m_GameObject;
}

dae::MoveCommand::MoveCommand(GameObject& gameObject, float speed, dae::Input::Direction direction) :
	GameActorCommand(gameObject),
	m_Speed{ speed },
	m_Direction{ direction }
{
}

void dae::MoveCommand::Execute()
{
	GameObject& gameObject = GetGameActor();
	glm::vec3 pos = gameObject.GetLocalPosition();
	float offset = m_Speed * Timer::GetInstance().GetDeltaTime();
	switch (m_Direction)
	{
	case Input::Direction::Up:
		pos.y -= offset; break;
	case Input::Direction::Down:
		pos.y += offset; break;
	case Input::Direction::Left:
		pos.x -= offset; break;
	case Input::Direction::Right:
		pos.x += offset; break;
	}
	gameObject.SetLocalPosition(pos);
}
