#include "Rendering/RendererComponentBase.h"
#include "Rendering/SDLRenderer.h"

namespace JRE
{
	RendererComponentBase::RendererComponentBase(GameObject& gameObject)
		: ComponentBase(gameObject)
	{
		//scene.Add(gameObject) will track all RendererComponents
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
