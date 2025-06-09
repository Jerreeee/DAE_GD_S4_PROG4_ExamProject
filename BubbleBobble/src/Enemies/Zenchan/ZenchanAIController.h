#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class ZenchanScriptComponent;

	class ZenchanAIController final : public JRE::ComponentBase
	{
	public:
		ZenchanAIController(JRE::GameObject& gameObject);

		void Update() override;

		const JRE::GameObject* m_pTarget{ nullptr };
	private:
		ZenchanScriptComponent* m_pZenchanScript{ nullptr };

		float m_JumpTimer{ 0.f };
		float m_JumpInterval{ 1.f };
		int m_CurrentDirection{ 1 };
	};
}
