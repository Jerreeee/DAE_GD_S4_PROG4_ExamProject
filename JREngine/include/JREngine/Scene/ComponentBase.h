#pragma once
#include <memory>
#include "JREngine/Scene/Transform.h"
#include "JREngine/Core/Timer.h"

namespace JRE
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

		virtual void Start() {};
		virtual void Update() = 0;

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

