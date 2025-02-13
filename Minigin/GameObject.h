#pragma once
#include <memory>
#include "TransformComponent.h"
#include "ComponentBase.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
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
		ComponentType* GetComponent()
		{
			for (const auto& component : m_Components)
			{
				if (auto castedComponent = dynamic_cast<ComponentType*>(component.get()))
				{
					return castedComponent;
				}
			}
			return nullptr;
		}

		void SetPosition(float x, float y);

		TransformComponent& GetTransform();
	private:
		TransformComponent m_Transform{};
		std::vector<std::unique_ptr<ComponentBase>> m_Components{};
	};
}
