#include "ComponentBase.h"
#include "GameObject.h"
#include "TransformComponent.h"

namespace dae
{
	ComponentBase::ComponentBase(GameObject& gameObject)
		: m_GameObject(gameObject)
		, m_Transform(gameObject.GetTransform())
	{
	}
}
