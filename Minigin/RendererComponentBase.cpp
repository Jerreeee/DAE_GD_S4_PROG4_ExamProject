#include "RendererComponentBase.h"
#include "Renderer.h"

dae::RendererComponentBase::RendererComponentBase(GameObject& gameObject)
	: ComponentBase(gameObject)
{
	Renderer::GetInstance().RegisterRendererComponent(this);
}

dae::RendererComponentBase::~RendererComponentBase()
{
	Renderer::GetInstance().UnRegisterRendererComponent(this);
}

void dae::RendererComponentBase::SetActive(bool active)
{
	m_Active = active;
}

bool dae::RendererComponentBase::IsActive() const
{
	return m_Active;
}
