#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"

#include "Enemies/Zenchan/ZenchanScriptComponent.h"

using namespace JRE;

namespace BubbleBobble
{
	ZenchanScriptComponent::ZenchanScriptComponent(JRE::GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		m_pSpriteAnimator = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		assert(m_pSpriteAnimator && "Player doesnt have SpriteAnimatorComponent");
		m_pTileMapComponent = SceneManager::GetInstance().GetCurrentScene().GetComponent<TileMapComponent>();
		assert(m_pTileMapComponent && "m_pTileMapComponent was nullptr");
	}

	void ZenchanScriptComponent::FixedUpdate()
	{
		float dt = Timer::GetInstance().GetFixedTimeStep();

		glm::vec3 oldPos = GetGameObject().GetWorldPosition();

		//Calc vel
		m_Vel.x = m_Speed * m_Input.moveDir;

		//Check TileMap collision
		glm::vec2 colliderPos{ oldPos.x + m_ColliderOffset.x, oldPos.y + m_ColliderOffset.y };
		Region collider{ colliderPos.x, colliderPos.y, m_ColliderSize.x, m_ColliderSize.y };

		m_pTileMapComponent->GetTileMap().MovePosition(collider, m_Vel, dt, true, m_CollInfo);
		GetGameObject().SetWorldPosition(m_CollInfo.newPos.x - m_ColliderOffset.x, m_CollInfo.newPos.y - m_ColliderOffset.y);
		m_Vel = m_CollInfo.velOut;

		//if (m_Input.moveDir)
		//	m_pSpriteAnimator->SetActiveClip("Run");
		//else
		//	m_pSpriteAnimator->SetActiveClip("RunAgry");
		//TODO Flip running animation depending on going left or right

		//bool shoot = im.IsBindingActive(*m_pActionMap, "Shoot");
		//if (shoot);
		//	return State::Shooting;
		bool onGround = m_CollInfo.collDir.down;
		if (onGround && m_Input.pressedJump)
			m_Vel.y = -m_JumpForce;

		m_Input = Input{}; //Consume all input
	}

	void ZenchanScriptComponent::Move(int direction)
	{
		m_Input.moveDir = std::clamp(direction, -1, 1);
	}

	void ZenchanScriptComponent::Jump()
	{
		if (!m_Input.pressedJump)
			m_Input.pressedJump = true;
	}
}
