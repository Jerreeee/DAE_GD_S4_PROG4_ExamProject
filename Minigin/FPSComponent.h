#pragma once
#include "ComponentBase.h"

namespace dae
{
	class FPSComponent final : public ComponentBase
	{
	public:
		FPSComponent(GameObject& gameObject, float updateRate = 1.f);

		virtual void Update() override;
	private:
		float m_UpdateRate{};
		float m_AccTime{};
		float m_FPS{};
	};
}

