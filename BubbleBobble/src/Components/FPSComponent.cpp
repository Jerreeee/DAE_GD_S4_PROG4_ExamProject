#include <format>
#include <sstream>
#include "SDL.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/Font.h"
#include "FPSComponent.h"

namespace BubbleBobble
{
	FPSComponent::FPSComponent(JRE::GameObject& gameObject, JRE::AssetHandle fontHandle, float updateRate)
		: ComponentBase(gameObject)
		,m_UpdateRate{ updateRate }
		,m_AccTime{ m_UpdateRate }
	{
		m_pTextRendererComponent = GetGameObject().AddComponent<JRE::TextRendererComponent>("", JRE::SoftAssetRef<JRE::Font>(fontHandle), SDL_Color{255, 255, 255, 255});
	}

	void FPSComponent::Update()
	{
		float dt = JRE::Timer::GetInstance().GetDeltaTime();
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
