#include <format>
#include <sstream>
#include "JREngine/GameObject.h"
#include "JREngine/Font.h"
#include "FPSComponent.h"

namespace BubbleBobble
{
	FPSComponent::FPSComponent(JREngine::GameObject& gameObject, std::shared_ptr<JREngine::Font> font, float updateRate)
		: ComponentBase(gameObject)
		,m_UpdateRate{ updateRate }
		,m_AccTime{ m_UpdateRate }
	{
		m_pTextRendererComponent = GetGameObject().AddComponent<JREngine::TextRendererComponent>("", font);
	}

	void FPSComponent::Update()
	{
		float dt = JREngine::Timer::GetInstance().GetDeltaTime();
		m_AccTime += dt;
		if (m_AccTime < m_UpdateRate)
			return;

		m_AccTime -= m_UpdateRate;
		m_FPS = 1.0f / dt;

		std::stringstream ss{};
		ss << std::format("{:.2f}", m_FPS) << " FPS";
		m_pTextRendererComponent->SetText(ss.str());
	}

	float FPSComponent::GetFPS()
	{
		return m_FPS;
	}

	void FPSComponent::SetUpdateRate(float updateRate)
	{
		m_UpdateRate = updateRate;
	}
}
