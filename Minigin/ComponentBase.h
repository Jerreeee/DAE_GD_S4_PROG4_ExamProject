#pragma once
#include <memory>

namespace dae
{
	class GameObject;
	class TransformComponent;
	class ComponentBase
	{
	public:
		ComponentBase(GameObject& gameObject);

		virtual ~ComponentBase() = default;
		ComponentBase(const ComponentBase& other) = delete;
		ComponentBase& operator=(const ComponentBase& other) = delete;
		ComponentBase(ComponentBase&& other) = delete;
		ComponentBase& operator=(ComponentBase&& other) = delete;

		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void Render() const {};
	protected:
		GameObject& m_GameObject;
		TransformComponent& m_Transform;
	};
}

