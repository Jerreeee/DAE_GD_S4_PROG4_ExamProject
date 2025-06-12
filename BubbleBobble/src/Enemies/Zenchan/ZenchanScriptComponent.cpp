#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Physics/Box2DColliderComponent.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Core/ServiceLocator.h"

#include "Utils.h"
#include "CollisionLayers.h"
#include "Enemies/Zenchan/ZenchanScriptComponent.h"

using namespace JRE;

namespace BubbleBobble
{
	ZenchanScriptComponent::ZenchanScriptComponent(JRE::GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		m_pSpriteRendererCmp = GetGameObject().GetComponent<SpriteRendererComponent>();
		assert(m_pSpriteRendererCmp && "Player doesnt have SpriteRendererComponent");
		m_pSpriteAnimatorCmp = GetGameObject().GetComponent<SpriteAnimatorComponent>();
		assert(m_pSpriteAnimatorCmp && "Player doesnt have SpriteAnimatorComponent");
		m_pBox2DColliderCmp = GetGameObject().GetComponent<Box2DColliderComponent>();
		assert(m_pBox2DColliderCmp && "Player doesnt have Box2DColliderComponent");
	}

	void ZenchanScriptComponent::FixedUpdate()
	{
		glm::vec3 oldPos = GetGameObject().GetWorldPosition();

		BoxPhysicsSystem::CollisionSettings cs{ oldPos, *m_pBox2DColliderCmp };
		cs.dt = Timer::GetInstance().GetFixedTimeStep();
		cs.applyGravity = true;
		cs.filterFunc = Utils::PlatformCollisionDirFilterFunc;

		//Calc vel
		m_Vel.x = m_Speed * m_Input.moveDir;
		cs.vel = m_Vel;

		//Check collision
		BoxPhysicsSystem& physicsSystem = static_cast<BoxPhysicsSystem&>(ServiceLocator::GetPhysicsSystem());
		physicsSystem.MoveCollider(cs, m_CollInfo);
		GetGameObject().SetWorldPosition(m_CollInfo.newPos.x, m_CollInfo.newPos.y);
		m_Vel = m_CollInfo.velOut;

		//Flip sprites based on movement direction
		if (m_Input.moveDir != 0)
			m_FacingDir = m_Input.moveDir;
		m_pSpriteRendererCmp->SetFlipX(m_FacingDir == -1);

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
