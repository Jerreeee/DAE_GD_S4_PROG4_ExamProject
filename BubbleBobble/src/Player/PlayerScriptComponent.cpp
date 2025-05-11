#include "JREngine/Scene/GameObject.h"
#include "JREngine/Input/InputManager.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"

#include "Player/PlayerScriptComponent.h"

#include <iostream>

namespace BubbleBobble
{
	PlayerScriptComponent::PlayerScriptComponent(JRE::GameObject& gameObject) :
		ComponentBase(gameObject),
		m_Player{ gameObject }
	{
		m_SpriteRendererComponent = m_Player.GetComponent<JRE::SpriteRendererComponent>();
		assert(m_SpriteRendererComponent && "m_SpriteRendererComponent was nullptr");

		auto pMoveLeftCommand = std::make_unique<MoveCommand>(*this, glm::vec2{ -1.f, 0.f });
		auto pMoveRightCommand = std::make_unique<MoveCommand>(*this, glm::vec2{ 1.f, 0.f });

		JRE::Input::InputManager& inputManager = JRE::Input::InputManager::GetInstance();
		size_t playerIdx = inputManager.AddPlayer();
		inputManager.BindCommand(playerIdx, std::move(pMoveLeftCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_LEFT, JRE::Input::ButtonState::Pressed })
					.BindCommand(playerIdx, std::move(pMoveRightCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_RIGHT, JRE::Input::ButtonState::Pressed });
	
		m_pState = std::make_unique<PlayerIdleState>(*this);
	}

	void PlayerScriptComponent::Update()
	{
		m_pState->OnHandleInput(m_Input);
		auto newState = m_pState->Update();
		if (newState)
		{
			m_pState->OnExit();
			m_pState = std::move(newState);
			m_pState->OnEnter();
		}

		//reset input for next frame
		m_Input = {};
	}

	void PlayerScriptComponent::Move(glm::vec2 direction)
	{
		glm::vec3 pos = m_Player.GetWorldPosition();
		const int pixelsPerM{ 10 };
		float offset = m_Speed * pixelsPerM * JRE::Timer::GetInstance().GetDeltaTime();
		pos.x += offset * direction.x;
		pos.y += offset * direction.y;
		m_Player.SetWorldPosition(pos);
	}
}
