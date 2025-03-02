#pragma once
#include "ComponentBase.h"

namespace dae
{
	class UIComponentBase : public ComponentBase
	{
	public:
		UIComponentBase(GameObject& gameObject);
		virtual ~UIComponentBase();

		virtual void Update() override {};
		virtual void UpdateAndRenderUI() = 0;
	};
}