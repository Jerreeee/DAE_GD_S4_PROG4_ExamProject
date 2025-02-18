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

		m_Children.erase(std::remove_if(m_Children.begin(), m_Children.end(),
			[&](const std::unique_ptr<GameObject>& child)
			{
				return std::find(m_ChildrenToRemove.begin(), m_ChildrenToRemove.end(), child.get()) != m_ChildrenToRemove.end();
			}),
			m_Children.end()
		);

		m_ComponentsToRemove.clear();
		m_ChildrenToRemove.clear();
	}


	void GameObject::SetPosition(float x, float y)
	{
		m_LocalTransform.SetPosition(x, y, 0.0f);
	}

	Transform& GameObject::GetTransform()
	{
		return m_LocalTransform;
	}
	GameObject* GameObject::GetParent() const
	{
		return m_pParent;
	}
	void GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
	{
		if (IsChild(pParent) || pParent == this)
			return;
		if (pParent == nullptr)
			SetLocalPosition(GetWorldPosition());
		else
		{
			if (keepWorldPosition)
				SetLocalPosition(GetWorldPosition() - m_pParent->GetWorldPosition());
			SetPositionDirty();
		}
		if (m_pParent)
			m_pParent->RemoveChild(this);
		m_pParent = pParent;
		if (m_pParent)
			m_pParent->AddChild(this);
	}
	size_t GameObject::GetChildCount() const
	{
		return m_Children.size();
	}
	GameObject* GameObject::GetChildAtIndex(size_t idx) const
	{
		return idx >= 0 && idx < m_Children.size() ? m_Children.at(idx).get() : nullptr;
	}
	bool GameObject::IsChild(GameObject* pChild, bool recursive) const
	{
		auto childIt = std::find_if(m_Children.begin(), m_Children.end(),
			[&](const std::unique_ptr<GameObject>& pGameObject) -> bool
			{
				return pGameObject.get() == pChild || (recursive && pGameObject->IsChild(pChild));
			}
		);
		return childIt != m_Children.end();
	}
	void GameObject::SetLocalPosition(const glm::vec3& position)
	{
		m_LocalTransform.SetPosition(position);
		SetPositionDirty();
	}
	const glm::vec3& GameObject::GetWorldPosition()
	{
			if (m_PositionIsDirty)
				UpdateWorldPosition();
			return m_WorldTransform.GetPosition();
	}
	void GameObject::UpdateWorldPosition()
	{
		if (!m_PositionIsDirty)
			return;

		if (m_pParent)
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition() + m_pParent->GetWorldPosition());
		else
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
		m_PositionIsDirty = false;
	}
	void GameObject::SetPositionDirty()
	{
		m_PositionIsDirty = true;
		for (const auto& child : m_Children)
			child->SetPositionDirty();
	}
	void GameObject::AddChild(GameObject* pChild)
	{
		m_Children.emplace_back(std::unique_ptr<GameObject>(pChild));
	}
	void GameObject::RemoveChild(GameObject* pChild)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(),
			[&](const std::unique_ptr<GameObject>& child) -> bool
			{
				return child.get() == pChild;
			}
		);
		if (it != m_Children.end())
			m_ChildrenToRemove.push_back(it->get());
	}
}
