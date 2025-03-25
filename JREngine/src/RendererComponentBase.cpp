#include "RendererComponentBase.h"
#include "Renderer.h"

JREngine::RendererComponentBase::RendererComponentBase(GameObject& gameObject)
	: ComponentBase(gameObject)
{
	Renderer::GetInstance().RegisterRendererComponent(this);
}

JREngine::RendererComponentBase::~RendererComponentBase()
{
	Renderer::GetInstance().UnRegisterRendererComponent(this);
}

void JREngine::RendererComponentBase::SetActive(bool active)
{
	m_Active = active;
}

bool JREngine::RendererComponentBase::IsActive() const
{
	return m_Active;
}
