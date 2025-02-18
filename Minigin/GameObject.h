#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"
#include "ComponentBase.h"
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace dae
{
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
		void Render() const;

		void Cleanup();

		template<typename ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args)
		{
			auto pComponent = std::make_unique<ComponentType>(*this, std::forward<Args>(args)...);
			ComponentType* rawPtr = pComponent.get();
			m_Components.emplace_back(std::move(pComponent));
			return rawPtr;
		}

		template<typename ComponentType>
		bool HasComponent()
		{
			return FindComponent<ComponentType>() != m_Components.end();
		}

		template<typename ComponentType>
		ComponentType* GetComponent()
		{
			auto componentIt = FindComponent<ComponentType>();
			return componentIt != m_Components.end() ? static_cast<ComponentType*>((*componentIt).get()) : nullptr;
		}

		template<typename ComponentType>
		void RemoveComponent()
		{
			auto it = FindComponent<ComponentType>();
			if (it != m_Components.end())
				m_ComponentsToRemove.push_back(it->get());
		}

		void SetPosition(float x, float y);

		TransformComponent* GetTransform();
	private:
		//Variables
		std::string m_Name{};
		TransformComponent m_Transform{};
		std::vector<std::unique_ptr<ComponentBase>> m_Components{};
		std::vector<ComponentBase*> m_ComponentsToRemove{};

		//Functions
		template<typename ComponentType>
		auto FindComponent() const
		{
			return std::find_if(m_Components.begin(), m_Components.end(), 
				[](const std::unique_ptr<ComponentBase>& component)
				{ return dynamic_cast<ComponentType*>(component.get()) != nullptr; }
			);
		}
	};
}
