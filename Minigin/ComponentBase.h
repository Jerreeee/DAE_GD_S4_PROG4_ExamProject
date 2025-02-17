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

		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void Render() const {};
	protected:
		GameObject& m_GameObject;
		TransformComponent& m_Transform;
	};
}

