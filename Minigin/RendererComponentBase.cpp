#include "RendererComponentBase.h"
#include "Renderer.h"

dae::RendererComponentBase::RendererComponentBase(GameObject& gameObject)
	: ComponentBase(gameObject)
{
	Renderer::GetInstance().RegisterRendererComponent(this);
}

void dae::RendererComponentBase::SetActive(bool active)
{
	m_Active = active;
	if (m_Active)
		Renderer::GetInstance().RegisterRendererComponent(this);
}

bool dae::RendererComponentBase::IsActive() const
{
	return m_Active;
}
