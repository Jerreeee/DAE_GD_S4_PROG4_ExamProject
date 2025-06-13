#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <concepts>
#include "JREngine/Scene/Transform.h"
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Scene/PersistenceLayer.h"

namespace JRE
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

		void Start();
		void Update();
		void FixedUpdate();
		void Cleanup();

		bool HasExecutedStart() const { return m_ExecutedStart; };
		void Destroy();
		bool IsDestroyed() const;
		void SetActive(bool active);
		bool IsActive() const { return m_IsActive; }
		bool IsActiveInHierarchy() const { return m_IsActiveInHierarchy; }

		Transform& GetWorldTransform();
		Transform& GetLocalTransform();
		const glm::vec3& GetWorldPosition() const;
		const glm::vec3& GetLocalPosition() const;
		void SetWorldPosition(float x, float y);
		void SetWorldPosition(const glm::vec3& pos);
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& position);

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
		const ComponentType* GetComponent() const
		{
			auto componentIt = FindComponent<ComponentType>();
			return componentIt != m_Components.end() ? static_cast<const ComponentType*>((*componentIt).get()) : nullptr;
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
		size_t GetComponentCount() const;
		ComponentBase* GetComponentAtIndex(size_t idx);

		uint32_t m_PersistenceScope{ PersistenceLayer::SceneLocal };
	private:
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
		void UpdateWorldPosition() const;
		void AddChild(GameObject* pChild);

		void UpdateActiveInHierarchy();

		//-----------------------
		//Variables
		//-----------------------

		bool m_ExecutedStart{};
		bool m_IsActive{ true };
		bool m_IsActiveInHierarchy{ false };
		bool m_IsDestroyed{ false };
		std::string m_Name{};
		Transform m_LocalTransform{};
		mutable Transform m_WorldTransform{};
		mutable bool m_PositionIsDirty{ false };
		std::vector<std::unique_ptr<ComponentBase>> m_Components{};
		GameObject* m_pParent{};
		std::vector<GameObject*> m_Children{};

		void RemoveChild(GameObject* pChild);
	};
}
