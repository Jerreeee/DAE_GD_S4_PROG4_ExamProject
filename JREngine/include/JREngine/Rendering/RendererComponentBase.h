#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class RendererComponentBase : public ComponentBase
	{
	public:
		RendererComponentBase(GameObject& gameObject) : ComponentBase(gameObject) {}
		virtual ~RendererComponentBase() = default;

		virtual void Update() override {};
		virtual void Render() const = 0;
		virtual void UpdateAndRenderUI() {};
	};
}