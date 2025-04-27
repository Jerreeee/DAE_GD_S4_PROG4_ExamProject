#include "RendererComponentBase.h"
#include "Renderer.h"

JRE::RendererComponentBase::RendererComponentBase(GameObject& gameObject)
	: ComponentBase(gameObject)
{
	Renderer::GetInstance().RegisterRendererComponent(this);
}

JRE::RendererComponentBase::~RendererComponentBase()
{
	Renderer::GetInstance().UnRegisterRendererComponent(this);
}

void JRE::RendererComponentBase::SetActive(bool active)
{
	m_Active = active;
}

bool JRE::RendererComponentBase::IsActive() const
{
	return m_Active;
}
