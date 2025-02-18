#pragma once
#include <memory>

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

		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void Render() const {};
	protected:
		ComponentBase(GameObject& gameObject);

		GameObject& m_GameObject;
		Transform& m_Transform;
	};
}

