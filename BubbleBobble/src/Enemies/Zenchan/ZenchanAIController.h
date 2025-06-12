#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class GameObject;
	class Box2DColliderComponent;
}

namespace BubbleBobble
{
	class ZenchanScriptComponent;

	class ZenchanAIController final : public JRE::ComponentBase
	{
	public:
		ZenchanAIController(JRE::GameObject& gameObject);

		void Update() override;

		void SetTarget(const JRE::GameObject& gameObject);
	private:
		ZenchanScriptComponent* m_pZenchanScript{ nullptr };
		JRE::Box2DColliderComponent* m_pZenchanBox2DCmp{ nullptr };
		const JRE::GameObject* m_pTarget{ nullptr };
		const JRE::Box2DColliderComponent* m_pTargetBox2DCmp{ nullptr };

		float m_JumpTimer{ 0.f };
		float m_JumpInterval{ 1.f };
		int m_CurrentDirection{ 1 };
	};
}
