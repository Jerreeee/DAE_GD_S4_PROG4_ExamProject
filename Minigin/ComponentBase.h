#pragma once
#include <memory>
#include "Transform.h"
#include "Timer.h"

namespace dae
{
	class GameObject;
	class Transform;
	class ComponentBase
	{
	public:
		virtual ~ComponentBase() = default;
		ComponentBase(const ComponentBase& other) = delete;
		ComponentBase(ComponentBase&& other) = delete;
		ComponentBase& operator=(const ComponentBase& other) = delete;
		ComponentBase& operator=(ComponentBase&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;

		void Destroy();
		bool IsDestroyed() const;
	protected:
		ComponentBase(GameObject& gameObject);
		GameObject& GetGameObject() const;
		Transform& GetWorldTransform() const;
		Transform& GetLocalTransform() const;
	private:
		GameObject& m_GameObject;
		bool m_IsDestroyed{ false };
	};
}

