#include "RendererComponentBase.h"
#include "Renderer.h"

Engine::RendererComponentBase::RendererComponentBase(GameObject& gameObject)
	: ComponentBase(gameObject)
{
	Renderer::GetInstance().RegisterRendererComponent(this);
}

Engine::RendererComponentBase::~RendererComponentBase()
{
	Renderer::GetInstance().UnRegisterRendererComponent(this);
}

void Engine::RendererComponentBase::SetActive(bool active)
{
	m_Active = active;
}

bool Engine::RendererComponentBase::IsActive() const
{
	return m_Active;
}
