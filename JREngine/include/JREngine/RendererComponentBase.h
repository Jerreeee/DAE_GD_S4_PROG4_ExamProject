#pragma once
#include "ComponentBase.h"

namespace JRE
{
	class RendererComponentBase : public ComponentBase
	{
	public:
		RendererComponentBase(GameObject& gameObject);
		virtual ~RendererComponentBase();

		virtual void Update() override {};
		virtual void Render() const = 0;
		virtual void UpdateAndRenderUI() {};

		void SetActive(bool active);
		bool IsActive() const;
	private:
		bool m_Active{ true };
	};
}