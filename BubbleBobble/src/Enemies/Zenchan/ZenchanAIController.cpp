#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/Timer.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

#include "Player/PlayerScriptComponent.h"
#include "ZenchanScriptComponent.h"
#include "ZenchanAIController.h"

#include <iostream>

using namespace JRE;

namespace BubbleBobble
{
	ZenchanAIController::ZenchanAIController(GameObject& gameObject)
		: ComponentBase(gameObject)
		, m_pZenchanScript{ gameObject.GetComponent<ZenchanScriptComponent>() }
		, m_pZenchanBox2DCmp{ gameObject.GetComponent<Box2DColliderComponent>() }
	{
		assert(m_pZenchanScript && "ZenchanAIController requires a ZenchanScriptComponent");
		assert(m_pZenchanBox2DCmp && "ZenchanAIController requires a Box2DColliderComponent");
	}

	void ZenchanAIController::Update()
	{
		if (!m_pTarget || !m_pZenchanScript)
			return;

		glm::vec3 zenchanPos = GetGameObject().GetWorldPosition();
		glm::vec3 playerPos = m_pTarget->GetWorldPosition();

		BoxShape zenchanBox = m_pZenchanBox2DCmp->GetBounds();
		BoxShape targetBox = m_pTargetBox2DCmp->GetBounds();

		float dx = targetBox.GetXDist(zenchanBox);
		float dy = targetBox.GetYDist(zenchanBox);

		float horizontalDist = std::abs(dx);

		float jumpXDistThreshold = 30.0f;

		const auto& coll = m_pZenchanScript->GetCollInfo();

		bool overlap = dx == 0.f && dy == 0.f;
		if (overlap)
			return;

		if (dy > 0.f) //above player
		{
			if (coll.collDir.down && 
				(m_CurrentDirection == -1 && coll.collDir.left) ||
				(m_CurrentDirection == 1 && coll.collDir.right))
			{
				m_CurrentDirection *= -1;
			}
			m_pZenchanScript->Move(m_CurrentDirection);
		}
		else
		{
			//move towards player in X
			m_CurrentDirection = (dx > 0.0f) ? 1 : -1;
			m_pZenchanScript->Move(m_CurrentDirection);

			if (dy < 0.f && horizontalDist < jumpXDistThreshold && coll.collDir.down)
				m_pZenchanScript->Jump();
		}
	}
	void ZenchanAIController::SetTarget(const JRE::GameObject& gameObject)
	{
		m_pTarget = &gameObject;
		m_pTargetBox2DCmp = m_pTarget->GetComponent<Box2DColliderComponent>();
		assert(m_pTargetBox2DCmp && "ZenchanAIController::SetTarget() | Provided gameObject doesnt have a Box2DColliderComponent");
	}
}
