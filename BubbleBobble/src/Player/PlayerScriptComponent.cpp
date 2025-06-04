#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Physics/RigidBody2DComponent.h"

#include "Player/PlayerScriptComponent.h"

#include <iostream>

namespace BubbleBobble::Player
{
	ScriptComponent::ScriptComponent(JRE::GameObject& gameObject, const JRE::Input::ActionMap& actionMap) :
		ComponentBase(gameObject),
		m_Player{ gameObject }
	{
		SetActionMapToUse(actionMap);

		m_pSpriteAnimatiorComponent = m_Player.GetComponent<JRE::SpriteAnimatorComponent>();
		if (!m_pSpriteAnimatiorComponent)
			m_pSpriteAnimatiorComponent = m_Player.AddComponent<JRE::SpriteAnimatorComponent>();

		m_pRigidBody2DComponent = m_Player.GetComponent<JRE::RigidBody2DComponent>();
		if (!m_pRigidBody2DComponent)
			m_pRigidBody2DComponent = m_Player.AddComponent<JRE::RigidBody2DComponent>();

		m_Animations.resize(AnimationName::s_Names.size());
		
		//Create all the different states
		m_States.emplace_back(std::make_unique<MovingState>(*this, m_pActionMap));
		m_States.emplace_back(std::make_unique<ShootState>(*this, m_pActionMap));
		m_States.emplace_back(std::make_unique<JumpState>(*this, m_pActionMap, 20.f));
		m_States.emplace_back(std::make_unique<DiedState>(*this, m_pActionMap));
		//Set MovingState by default
		m_pState = m_States[0].get();
	}

	void ScriptComponent::Start()
	{
		m_pState->OnEnter();
	}

	void ScriptComponent::Update()
	{
		auto newState = m_pState->Update();
		if (newState == State::None)
			return;

		m_pState->OnExit();
		size_t idx = static_cast<size_t>(newState);
		std::cout << "Changed to: " << s_StateNames[idx] << "\n";
		m_pState = m_States[idx].get();
		m_pState->OnEnter();
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
	void ScriptComponent::Jump(float force)
	{
		if (m_pRigidBody2DComponent)
			m_pRigidBody2DComponent->Launch(glm::vec2{ 0.f, force });
	}
	void ScriptComponent::SetActionMapToUse(const JRE::Input::ActionMap& actionMap)
	{
		m_pActionMap = &actionMap;
	}
	void ScriptComponent::SetAnimation(const std::string& animName, JRE::AssetRef<JRE::SpriteAnimationClip> clip)
	{
		Animation anim = AnimationName::GetAnimation(animName);
		switch (anim)
		{
		case Animation::Shoot:
			//auto pShootState = static_cast<ShootState*>(m_States[static_cast<size_t>(State::Shooting)].get());
			//pShootState->SetAnimationClip(clip);
			break;
		}
		m_Animations[static_cast<size_t>(anim)] = clip;
	}
	void ScriptComponent::ChangeAnimation(Animation anim)
	{
		m_pSpriteAnimatiorComponent->SetSpriteAnimationClip(m_Animations[static_cast<size_t>(anim)]);
	}
}
