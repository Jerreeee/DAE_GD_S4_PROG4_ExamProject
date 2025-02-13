#include "FPSComponent.h"
#include "Time.h"
#include "GameObject.h"
#include "TextRendererComponent.h"
#include <format>
#include <sstream>

namespace dae
{
	FPSComponent::FPSComponent(GameObject& gameObject, float updateRate)
		: ComponentBase(gameObject)
		,m_UpdateRate{ updateRate }
		,m_AccTime{ m_UpdateRate }
	{
	}

	void FPSComponent::Update()
	{
		float dt = Time::GetInstance().GetDeltaTime();
		m_AccTime += dt;
		if (m_AccTime < m_UpdateRate)
			return;

		m_AccTime -= m_UpdateRate;
		m_FPS = 1.0f / dt;
		if (m_GameObject.HasComponent<dae::TextRendererComponent>())
		{
			std::stringstream ss{};
			ss << std::format("{:.2f}", m_FPS) << " FPS";
			m_GameObject.GetComponent<dae::TextRendererComponent>().SetText(ss.str());
		}
	}
}
