#pragma once
#include "GameObject.h"
#include "InputUtils.h"

namespace dae
{
	class Command
	{
	public:
		struct Binding
		{
			std::unique_ptr<Command> command;
			Input::KeyState keyState;
		};

		virtual ~Command() = default;
		virtual void Execute() = 0;
	private:
	};

	class GameActorCommand : public Command
	{
	public:
		GameActorCommand(GameObject& gameObject);
		virtual ~GameActorCommand() = default;
	protected:
		GameObject& GetGameActor() const;
	private:
		GameObject& m_GameObject;
	};

	//Temporarily in here, should be in game files
	class MoveCommand final: public GameActorCommand
	{
	public:
		MoveCommand(GameObject& gameObject, float speed, dae::Input::Direction direction);

		virtual void Execute() override;
	private:
		float m_Speed{};
		Input::Direction m_Direction{};
	};
}