#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace dae
{
	GameObject::GameObject(const std::string& name)
		: m_Name(name)
	{
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

	void GameObject::Cleanup()
	{
		m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(),
			[&](const std::unique_ptr<ComponentBase>& component)
			{
				return std::find(m_ComponentsToRemove.begin(), m_ComponentsToRemove.end(), component.get()) != m_ComponentsToRemove.end();
			}),
			m_Components.end()
		);
		m_ComponentsToRemove.clear();
	}

	void GameObject::SetPosition(float x, float y)
	{
		m_Transform.SetPosition(x, y, 0.0f);
	}

	TransformComponent* GameObject::GetTransform()
	{
		return &m_Transform;
	}
}
