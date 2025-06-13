#pragma once
#include <memory>
#include "JREngine/Scene/Transform.h"
#include "JREngine/Core/Timer.h"

namespace JRE
{
	class GameObject;
	class ComponentBase
	{
	public:
		virtual ~ComponentBase() = default;
		ComponentBase(const ComponentBase& other) = delete;
		ComponentBase(ComponentBase&& other) = delete;
		ComponentBase& operator=(const ComponentBase& other) = delete;
		ComponentBase& operator=(ComponentBase&& other) = delete;

		virtual void Start() {};
		virtual void OnEnable() {};
		virtual void Update() = 0;
		virtual void FixedUpdate() {};
		virtual void OnDisable() {};

		void SetActive(bool active);
		bool IsActive() const;
		void Destroy();
		bool IsDestroyed() const;

		GameObject& GetGameObject() const;
		Transform& GetWorldTransform() const;
		Transform& GetLocalTransform() const;
	protected:
		ComponentBase(GameObject& gameObject);
	private:
		GameObject& m_GameObject;
		bool m_Active{ true };
		bool m_IsDestroyed{ false };
	};
}

