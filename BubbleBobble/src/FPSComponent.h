#pragma once
#include "JREngine/ComponentBase.h"
#include "JREngine/TextRendererComponent.h"
#include "JREngine/ResourceHandle.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class Font;
	class FPSComponent final : public JRE::ComponentBase
	{
	public:
		FPSComponent(JRE::GameObject& gameObject, JRE::ResourceHandle<JRE::Font> fontHandle, float updateRate = 1.f);

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
		JRE::TextRendererComponent* m_pTextRendererComponent{};
	};
}

