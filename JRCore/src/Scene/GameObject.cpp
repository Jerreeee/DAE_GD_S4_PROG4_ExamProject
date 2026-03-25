#include <string>
#include "Scene/GameObject.h"
#include "Asset/ResourceManager.h"

namespace JRE
{
	GameObject::GameObject(const std::string& name)
		: m_Name(name)
	{
	}

	void GameObject::Start()
	{
		if (m_ExecutedStart) return;
		for (const auto& component : m_Components)
			component->Start();
		//TODO take into acount component->IsEnabled()
		m_ExecutedStart = true;
	}

	void GameObject::Update()
	{
		for (const auto& component : m_Components)
			if (component->IsEnabled())
				component->Update();
	}

	void GameObject::FixedUpdate()
	{
		for (const auto& component : m_Components)
			if (component->IsEnabled())
				component->FixedUpdate();
	}

	void GameObject::Cleanup()
	{
		for (auto& component : m_Components)
			if (component->IsDestroyed())
				component->OnDisable();

		std::erase_if(m_Components, [](const std::unique_ptr<ComponentBase>& component)
			{
				return component->IsDestroyed();
			});
	}

	void GameObject::Destroy()
	{
		m_IsDestroyed = true;
		for (const auto& child : m_Children)
			child->Destroy();
	}

	bool GameObject::IsDestroyed() const
	{
		return m_IsDestroyed;
	}

	void GameObject::SetActive(bool active)
	{
		if (m_IsActive == active && m_IsActiveInHierarchy)
			return;

		m_IsActive = active;
		UpdateActiveInHierarchy();
	}

	void GameObject::SetLocalPosition(float x, float y)
	{
		m_LocalTransform.SetPosition(x, y, 0.0f);
	}

	Transform& GameObject::GetWorldTransform()
	{
		if (m_PositionIsDirty)
			UpdateWorldPosition();
		return m_WorldTransform;
	}

	Transform& GameObject::GetLocalTransform()
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

		UpdateActiveInHierarchy();
	}
	size_t GameObject::GetChildCount() const
	{
		return m_Children.size();
	}
	GameObject* GameObject::GetChildAtIndex(size_t idx) const
	{
		return idx >= 0 && idx < m_Children.size() ? m_Children.at(idx) : nullptr;
	}
	bool GameObject::IsChild(GameObject* pChild, bool recursive) const
	{
		auto childIt = std::find_if(m_Children.begin(), m_Children.end(),
			[&](const GameObject* pGameObject) -> bool
			{
				return pGameObject == pChild || (recursive && pGameObject->IsChild(pChild));
			}
		);
		return childIt != m_Children.end();
	}
	size_t GameObject::GetComponentCount() const
	{
		return m_Components.size();
	}
	ComponentBase* GameObject::GetComponentAtIndex(size_t idx)
	{
		return idx >= 0 && idx < m_Components.size() ? m_Components.at(idx).get() : nullptr;
	}
	void GameObject::SetLocalPosition(const glm::vec3& position)
	{
		m_LocalTransform.SetPosition(position);
		SetPositionDirty();
	}
	const glm::vec3& GameObject::GetWorldPosition() const
	{
		if (m_PositionIsDirty)
			UpdateWorldPosition();
		return m_WorldTransform.GetPosition();
	}
	const glm::vec3& GameObject::GetLocalPosition() const
	{
		return m_LocalTransform.GetPosition();
	}
	void GameObject::SetWorldPosition(float x, float y)
	{
		m_WorldTransform.SetPosition(x, y, 0.f);
	}
	void GameObject::SetWorldPosition(const glm::vec3& pos)
	{
		m_WorldTransform.SetPosition(pos);
	}
	void GameObject::UpdateWorldPosition() const
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
		m_Children.emplace_back(pChild);
	}
	void GameObject::UpdateActiveInHierarchy()
	{
		bool parentActive = m_pParent ? m_pParent->IsActiveInHierarchy() : true;
		bool newActive = m_IsActive && parentActive;

		if (newActive == m_IsActiveInHierarchy) return;

		m_IsActiveInHierarchy = newActive;

		if (m_IsActiveInHierarchy)
		{
			if (!m_ExecutedStart)
				Start();

			for (auto& comp : m_Components)
				comp->OnEnable();
		}
		else
		{
			for (auto& comp : m_Components)
				comp->OnDisable();
		}

		for (auto* child : m_Children)
			child->UpdateActiveInHierarchy();
	}
	void GameObject::RemoveChild(GameObject* pChild)
	{
		auto it = std::find_if(m_Children.begin(), m_Children.end(),
			[&](const GameObject* pCurrentChild) -> bool
			{
				return pCurrentChild == pChild;
			}
		);
		if (it != m_Children.end())
			(*it)->Destroy();
	}
}
