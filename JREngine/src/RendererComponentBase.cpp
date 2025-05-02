#include "RendererComponentBase.h"
#include "Renderer.h"

JRE::RendererComponentBase::RendererComponentBase(GameObject& gameObject)
	: ComponentBase(gameObject)
{
	SDLRenderer::GetInstance().RegisterRendererComponent(this);
}

JRE::RendererComponentBase::~RendererComponentBase()
{
	SDLRenderer::GetInstance().UnRegisterRendererComponent(this);
}

void JRE::RendererComponentBase::SetActive(bool active)
{
	m_Active = active;
}

bool JRE::RendererComponentBase::IsActive() const
{
	return m_Active;
}
