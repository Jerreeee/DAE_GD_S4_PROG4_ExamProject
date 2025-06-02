#include "JREngine/Scene/GameObject.h"
#include "JREngine/Input/InputManager.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Physics/RigidBody2DComponent.h"

#include "Player/PlayerScriptComponent.h"

#include <iostream>

namespace BubbleBobble::Player
{
	ScriptComponent::ScriptComponent(JRE::GameObject& gameObject) :
		ComponentBase(gameObject),
		m_Player{ gameObject }
	{
		m_pSpriteAnimatiorComponent = m_Player.GetComponent<JRE::SpriteAnimatorComponent>();
		if (!m_pSpriteAnimatiorComponent)
			m_pSpriteAnimatiorComponent = m_Player.AddComponent<JRE::SpriteAnimatorComponent>();

		m_pRigidBody2DComponent = m_Player.GetComponent<JRE::RigidBody2DComponent>();
		if (!m_pRigidBody2DComponent)
			m_pRigidBody2DComponent = m_Player.AddComponent<JRE::RigidBody2DComponent>();

		m_Animations.resize(AnimationName::s_Names.size());
		
		//Create all the different states
		m_States.emplace_back(std::make_unique<MovingState>(*this));
		m_States.emplace_back(std::make_unique<ShootState>(*this));
		m_States.emplace_back(std::make_unique<JumpState>(*this));
		m_States.emplace_back(std::make_unique<DiedState>(*this));
		//Set MovingState by default
		m_pState = m_States[0].get();

		//Setup commands to collect input from the InputManager (could also poll inside the state itself)
		auto pMoveLeftCommand = std::make_unique<MoveCommand>(*this, -1);
		auto pMoveRightCommand = std::make_unique<MoveCommand>(*this, 1);
		auto pJumpCommand = std::make_unique<JumpCommand>(*this);
		auto pShootCommand = std::make_unique<ShootCommand>(*this);

		JRE::Input::InputManager& inputManager = JRE::Input::InputManager::GetInstance();
		size_t playerIdx = inputManager.AddActionMap();
		inputManager.BindCommand(playerIdx, std::move(pMoveLeftCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_LEFT, JRE::Input::ButtonState::Pressed })
			.BindCommand(playerIdx, std::move(pMoveRightCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::DPAD_RIGHT, JRE::Input::ButtonState::Pressed })
			.BindCommand(playerIdx, std::move(pJumpCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::FACE_DOWN, JRE::Input::ButtonState::DownThisFrame })
			.BindCommand(playerIdx, std::move(pShootCommand), JRE::Input::ControllerBindingInfo{ JRE::Input::ControllerButton::FACE_UP, JRE::Input::ButtonState::DownThisFrame });

	}

	void ScriptComponent::Start()
	{
		m_pState->OnEnter();
	}

	void ScriptComponent::Update()
	{
		auto newState = m_pState->Update();
		if (newState != State::None)
		{
			m_pState->OnExit();
			size_t idx = static_cast<size_t>(newState);
			std::cout << "Changed to: " << s_StateNames[idx] << "\n";
			m_pState = m_States[idx].get();
			m_pState->OnEnter();
		}

		//reset input for next frame
		m_Input = {};
	}

	void ScriptComponent::Move(int direction)
	{
		direction = std::clamp(direction, -1, 1);

		glm::vec3 pos = m_Player.GetWorldPosition();
		const int pixelsPerM{ 10 };
		float offset = m_Speed * pixelsPerM * JRE::Timer::GetInstance().GetDeltaTime();
		pos.x += offset * direction;
		m_Player.SetWorldPosition(pos);
	}
	void ScriptComponent::Jump()
	{
		if (m_pRigidBody2DComponent)
			m_pRigidBody2DComponent->Launch(glm::vec2{ 0.f, m_JumpForce });
	}
	void ScriptComponent::SetAnimation(const std::string& animName, JRE::AssetRef<JRE::SpriteAnimationClip> clip)
	{
		Animation anim = AnimationName::GetAnimation(animName);
		switch (anim)
		{
		case Animation::Shoot:
			auto pShootState = static_cast<ShootState*>(m_States[static_cast<size_t>(State::Shooting)].get());
			pShootState->SetAnimationClip(clip);
			break;
		}
		m_Animations[static_cast<size_t>(anim)] = clip;
	}
	void ScriptComponent::ChangeAnimation(Animation anim)
	{
		m_pSpriteAnimatiorComponent->SetSpriteAnimationClip(m_Animations[static_cast<size_t>(anim)]);
	}
}
