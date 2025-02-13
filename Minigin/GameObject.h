#pragma once
#include <memory>
#include "TransformComponent.h"
#include "ComponentBase.h"
#include <vector>
#include <string>
#include <stdexcept>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		GameObject(const std::string& name);

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void FixedUpdate();
		virtual void Update();
		virtual void Render() const;

		template<typename ComponentType, typename... Args>
		void AddComponent(Args&&... args)
		{
			m_Components.emplace_back(std::make_unique<ComponentType>(*this, std::forward<Args>(args)...));
		}

		template<typename ComponentType>
		bool HasComponent()
		{
			for (const auto& component : m_Components)
				if (auto castedComponent = dynamic_cast<ComponentType*>(component.get()))
					return true;
			return false;
		}

		template<typename ComponentType>
		ComponentType& GetComponent()
		{
			for (const auto& component : m_Components)
				if (auto castedComponent = dynamic_cast<ComponentType*>(component.get()))
					return *castedComponent;
			throw std::runtime_error("Component not found");
		}

		template<typename ComponentType>
		void RemoveComponent()
		{
			auto it = std::remove_if(m_Components.begin(), m_Components.end(),
				[](const std::unique_ptr<ComponentBase>& component)
				{
					return dynamic_cast<ComponentType*>(component.get()) != nullptr;
				}
			);

			if (it != m_Components.end())
			{
				m_Components.erase(it);
			}
		}

		void SetPosition(float x, float y);

		TransformComponent& GetTransform();
	private:
		std::string m_Name{};
		TransformComponent m_Transform{};
		std::vector<std::unique_ptr<ComponentBase>> m_Components{};
	};
}
