#include "JREngine/Scene/GameObject.h"
#include "JREngine/Input/InputManager.h"
#include "Player/MoveCommand.h"
#include "Player/PlayerScriptComponent.h"

namespace BubbleBobble
{
	PlayerScriptComponent::PlayerScriptComponent(JRE::GameObject& gameObject) :
		ComponentBase(gameObject)
	{
		auto& obj = GetGameObject();

		m_pMoveUpCommand = std::make_unique<MoveCommand>(obj, 100.f, glm::vec2{ 0.f, -1.f });
		m_pMoveDownCommand = std::make_unique<MoveCommand>(obj, 100.f, glm::vec2{ 0.f, 1.f });
		m_pMoveLeftCommand = std::make_unique<MoveCommand>(obj, 100.f, glm::vec2{ -1.f, 0.f });
		m_pMoveRightCommand = std::make_unique<MoveCommand>(obj, 100.f, glm::vec2{ 1.f, 0.f });

		JRE::Input::InputManager& inputManager = JRE::Input::InputManager::GetInstance();
		size_t playerIdx = inputManager.AddPlayer();
		inputManager.BindCommand(playerIdx, std::move(m_pMoveUpCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_UP, JRE::Input::ButtonState::Pressed })
					.BindCommand(playerIdx, std::move(m_pMoveDownCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_DOWN, JRE::Input::ButtonState::Pressed })
					.BindCommand(playerIdx, std::move(m_pMoveLeftCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_LEFT, JRE::Input::ButtonState::Pressed })
					.BindCommand(playerIdx, std::move(m_pMoveRightCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_RIGHT, JRE::Input::ButtonState::Pressed });
	}

	void PlayerScriptComponent::Update()
	{

	}
}
