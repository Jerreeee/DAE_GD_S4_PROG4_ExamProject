#pragma once
#include "ComponentBase.h"

namespace dae
{
	class FPSComponent final : public ComponentBase
	{
	public:
		FPSComponent(GameObject& gameObject);

		virtual void Update() override;
	private:
		float m_FPS{};
	};
}

