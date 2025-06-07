#include <string>
#include "Scene/GameObject.h"
#include "Asset/ResourceManager.h"
#include "Rendering/SDLRenderer.h"

namespace JRE
{
	GameObject::GameObject(const std::string& name)
		: m_Name(name)
	{
	}

	void GameObject::Start()
	{
		if (m_ExecutedStart) return;

		//TODO not ideal because Start() is only used by ScriptComponents
		//but simple fix for now
		for (const auto& component : m_Components)
			component->Start();
		m_ExecutedStart = true;
	}

	void GameObject::Update()
	{
		for (const auto& component : m_Components)
			component->Update();
	}

	void GameObject::FixedUpdate()
	{
		for (const auto& component : m_Components)
			component->FixedUpdate();
	}

	void GameObject::Cleanup()
	{
		m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(),
			[](const std::unique_ptr<ComponentBase>& component)
			{
				return component->IsDestroyed();
			}
		), m_Components.end());
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
	const glm::vec3& GameObject::GetLocalPosition()
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
		m_Children.emplace_back(pChild);
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
