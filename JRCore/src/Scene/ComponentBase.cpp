#include "Scene/GameObject.h"
#include "Scene/ComponentBase.h"

namespace JRE
{
	ComponentBase::ComponentBase(GameObject& gameObject)
		: m_GameObject(gameObject)
	{
	}
	GameObject& ComponentBase::GetGameObject() const
	{
		return m_GameObject;
	}

	void ComponentBase::SetEnabled(bool enable)
	{
		if (m_IsEnabled == enable)
			return;

		m_IsEnabled = enable;

		if (m_IsEnabled)
			OnEnable();
		else
			OnDisable();
	}

	void ComponentBase::Destroy()
	{
		m_IsDestroyed = true;
	}
	bool ComponentBase::IsDestroyed() const
	{
		return m_IsDestroyed;
	}
	Transform& ComponentBase::GetWorldTransform() const
	{
		return m_GameObject.GetWorldTransform();
	}
	Transform& ComponentBase::GetLocalTransform() const
	{
		return m_GameObject.GetLocalTransform();
	}
}
