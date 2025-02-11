#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace dae
{
	void GameObject::FixedUpdate()
	{
		for (const auto& component : m_Components)
		{
			component->FixedUpdate();
		}
	}

	void GameObject::Update()
	{
		for (const auto& component : m_Components)
		{
			component->Update();
		}
	}

	void GameObject::Render() const
	{
		for (const auto& component : m_Components)
		{
			component->Render();
		}
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_Transform.SetPosition(x, y, 0.0f);
	}

	TransformComponent& GameObject::GetTransform()
	{
		return m_Transform;
	}
}
