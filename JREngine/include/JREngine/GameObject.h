#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "ComponentBase.h"
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <concepts>

namespace JREngine
{
	template<typename T>
	concept DerivedFromComponentBase = std::is_base_of_v<ComponentBase, T>;

	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		GameObject(const std::string& name);

		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void Cleanup();
		void Destroy();
		bool IsDestroyed() const;

		template<DerivedFromComponentBase ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args)
		{
			auto pComponent = std::make_unique<ComponentType>(*this, std::forward<Args>(args)...);
			ComponentType* rawPtr = pComponent.get();
			m_Components.emplace_back(std::move(pComponent));
			return rawPtr;
		}

		template<DerivedFromComponentBase ComponentType>
		bool HasComponent()
		{
			return FindComponent<ComponentType>() != m_Components.end();
		}

		template<DerivedFromComponentBase ComponentType>
		ComponentType* GetComponent()
		{
			auto componentIt = FindComponent<ComponentType>();
			return componentIt != m_Components.end() ? static_cast<ComponentType*>((*componentIt).get()) : nullptr;
		}

		template<DerivedFromComponentBase ComponentType>
		void RemoveComponent()
		{
			auto it = FindComponent<ComponentType>();
			if (it != m_Components.end())
				it->get()->Destroy();
		}

		GameObject* GetParent() const;
		void SetParent(GameObject* pParent, bool keepWorldPosition = false);
		size_t GetChildCount() const;
		GameObject* GetChildAtIndex(size_t idx) const;
		bool IsChild(GameObject* pChild, bool recursive = true) const;

		Transform& GetWorldTransform();
		Transform& GetLocalTransform();
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition();
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& position);

	private:
		//-----------------------
		//Variables
		//-----------------------

		bool m_IsDestroyed{ false };	
		std::string m_Name{};
		Transform m_LocalTransform{};
		Transform m_WorldTransform{};
		bool m_PositionIsDirty{ true };
		std::vector<std::unique_ptr<ComponentBase>> m_Components{};
		GameObject* m_pParent{};
		std::vector<GameObject*> m_Children{};
		 
		//-----------------------
		//Functions
		//-----------------------

		template<DerivedFromComponentBase ComponentType>
		auto FindComponent() const
		{
			return std::find_if(m_Components.begin(), m_Components.end(), 
				[](const std::unique_ptr<ComponentBase>& component)
				{
					return dynamic_cast<ComponentType*>(component.get()) != nullptr;
				}
			);
		}

		void SetPositionDirty();
		void UpdateWorldPosition();
		void AddChild(GameObject* pChild);
		void RemoveChild(GameObject* pChild);
	};
}
