#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Rendering/RendererComponentBase.h"

namespace JRE
{
	RendererComponentBase::RendererComponentBase(GameObject& gameObject)
		: ComponentBase(gameObject)
	{
	}
}
