#include <iostream>
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Physics/RigidBody2DComponent.h"
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"

#include "TileMap/TileMapComponent.h"
#include "Player/PlayerScriptComponent.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble::Player
{
	ScriptComponent::ScriptComponent(JRE::GameObject& gameObject, const JRE::Input::ActionMap& actionMap) :
		ComponentBase(gameObject),
		m_Player{ gameObject }
	{
		SetActionMapToUse(actionMap);

		m_pSpriteAnimatiorComponent = m_Player.GetComponent<JRE::SpriteAnimatorComponent>();
		assert(m_pSpriteAnimatiorComponent && "m_pSpriteAnimatiorComponent was nullptr");
		m_pTileMapComponent = SceneManager::GetInstance().GetCurrentScene().GetComponent<TileMapComponent>();
		assert(m_pTileMapComponent && "m_pTileMapComponent was nullptr");

		m_Animations.resize(AnimationName::s_Names.size());
	}

	void ScriptComponent::Update()
	{
		auto& im = InputManager::GetInstance();
		if (!m_Input.pressedJump && im.IsBindingActive(*m_pActionMap, "Jump"))
			m_Input.pressedJump = true;
		if (!m_Input.movingLeft && im.IsBindingActive(*m_pActionMap, "MoveLeft"))
			m_Input.movingLeft = true;
		if (!m_Input.movingRight && im.IsBindingActive(*m_pActionMap, "MoveRight"))
			m_Input.movingRight = true;
	}

	void ScriptComponent::FixedUpdate()
	{
		float dt = Timer::GetInstance().GetFixedTimeStep();

		int direction = 0;
		if (m_Input.movingLeft)
			--direction;
		if (m_Input.movingRight)
			++direction;

		glm::vec3 oldPos = GetGameObject().GetWorldPosition();

		//Calc vel
		m_Vel.x = m_Speed * direction;

		//Check TileMap collision
		CollisionInfo m_CollInfo{};
		glm::vec2 colliderPos{ oldPos.x + m_ColliderOffset.x, oldPos.y + m_ColliderOffset.y };
		Region collider{ colliderPos.x, colliderPos.y, m_ColliderSize.x, m_ColliderSize.y };

		m_pTileMapComponent->GetTileMap().MovePosition(collider, m_Vel, dt, true, m_CollInfo);
		GetGameObject().SetWorldPosition(m_CollInfo.newPos.x - m_ColliderOffset.x, m_CollInfo.newPos.y - m_ColliderOffset.y);
		m_Vel = m_CollInfo.velOut;

		if (m_Input.movingLeft || m_Input.movingRight)
			ChangeAnimation(Animation::Idle);
		else
			ChangeAnimation(Animation::Run);
		//TODO Flip running animation depending on going left or right

		//bool shoot = im.IsBindingActive(*m_pActionMap, "Shoot");
		//if (shoot);
		//	return State::Shooting;
		bool onGround = m_CollInfo.collDir.down;
		if (onGround && m_Input.pressedJump)
			m_Vel.y = -m_JumpForce;

		m_Input = Input{}; //Consume all input
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
