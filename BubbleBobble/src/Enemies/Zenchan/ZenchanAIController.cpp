#include "JREngine/Scene/GameObject.h"
#include "JREngine/Core/Timer.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"

#include "Player/PlayerScriptComponent.h"
#include "ZenchanScriptComponent.h"
#include "ZenchanAIController.h"

#include <iostream>

using namespace JRE;

namespace BubbleBobble
{
	ZenchanAIController::ZenchanAIController(GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		m_pZenchanScript = gameObject.GetComponent<ZenchanScriptComponent>();
		assert(m_pZenchanScript && "ZenchanAIController requires ZenchanScriptComponent");
	}

	void ZenchanAIController::Update()
	{
		if (!m_pTarget || !m_pZenchanScript)
			return;

		glm::vec3 zenchanPos = GetGameObject().GetWorldPosition();
		glm::vec3 playerPos = m_pTarget->GetWorldPosition();

		float dx = playerPos.x - zenchanPos.x;
		float dy = playerPos.y - zenchanPos.y;

		float horizontalDist = std::abs(dx);
		float verticalDist = std::abs(dy);

		float xThreshold = 40.0f;
		float yThreshold = 20.0f;

		const auto& coll = m_pZenchanScript->GetCollInfo();

		if (horizontalDist <= xThreshold && verticalDist <= yThreshold)
			return;

		if (dy >= 0.0f)
		{
			if ((m_CurrentDirection == -1 && coll.collDir.left) ||
				(m_CurrentDirection == 1 && coll.collDir.right))
			{
				m_CurrentDirection *= -1;
			}
			m_pZenchanScript->Move(m_CurrentDirection);
		}
		else
		{
			int direction = (dx > 0.0f) ? 1 : -1;
			m_pZenchanScript->Move(direction);

			if (horizontalDist <= xThreshold && coll.collDir.down)
			{
				m_pZenchanScript->Jump();
			}
		}
	}
}
