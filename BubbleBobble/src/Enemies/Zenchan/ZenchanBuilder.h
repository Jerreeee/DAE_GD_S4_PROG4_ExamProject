#pragma once
#include <memory>

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class ZenchanBuilder final
	{
	public:
		ZenchanBuilder& SetupAIController(const JRE::GameObject* pTarget);
		void Build(std::unique_ptr<JRE::GameObject>& zenchan);
	private:
		const JRE::GameObject* m_pTarget{ nullptr };
	};
}