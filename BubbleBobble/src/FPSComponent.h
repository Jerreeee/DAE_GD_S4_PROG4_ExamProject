#pragma once
#include "JREngine/ComponentBase.h"
#include "JREngine/TextRendererComponent.h"

namespace JREngine
{
	class GameObject;
}

namespace BubbleBobble
{
	class Font;
	class FPSComponent final : public JREngine::ComponentBase
	{
	public:
		FPSComponent(JREngine::GameObject& gameObject, std::shared_ptr<JREngine::Font> font, float updateRate = 1.f);

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Update() override;

		float GetFPS();
		void SetUpdateRate(float updateRate);
	private:
		float m_UpdateRate{};
		float m_AccTime{};
		float m_FPS{};
		JREngine::TextRendererComponent* m_pTextRendererComponent{};
	};
}

