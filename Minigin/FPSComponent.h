#pragma once
#include "ComponentBase.h"
#include "TextRendererComponent.h"

namespace dae
{
	class Font;
	class FPSComponent final : public ComponentBase
	{
	public:
		FPSComponent(GameObject& gameObject, std::shared_ptr<Font> font, float updateRate = 1.f);

		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override {};

		float GetFPS();
		void SetUpdateRate(float updateRate);
	private:
		float m_UpdateRate{};
		float m_AccTime{};
		float m_FPS{};
		TextRendererComponent* m_pTextRendererComponent{};
	};
}

