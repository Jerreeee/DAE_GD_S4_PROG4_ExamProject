#include "Rendering/RendererComponentBase.h"
#include "Rendering/Renderer.h"

namespace JRE
{
	RendererComponentBase::RendererComponentBase(GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		SDLRenderer::GetInstance().RegisterRendererComponent(this);
	}

	RendererComponentBase::~RendererComponentBase()
	{
		SDLRenderer::GetInstance().UnRegisterRendererComponent(this);
	}

	void RendererComponentBase::SetActive(bool active)
	{
		m_Active = active;
	}

	bool RendererComponentBase::IsActive() const
	{
		return m_Active;
	}
}
