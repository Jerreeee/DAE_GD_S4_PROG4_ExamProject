#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class RotateParentComponent final: public JRE::ComponentBase
	{
	public:
		RotateParentComponent(JRE::GameObject& gameObject, float radius, float rotSpeedDeg);
		
		RotateParentComponent(const RotateParentComponent& other) = delete;
		RotateParentComponent(RotateParentComponent&& other) = delete;
		RotateParentComponent& operator=(const RotateParentComponent& other) = delete;
		RotateParentComponent& operator=(RotateParentComponent&& other) = delete;

		virtual void Update() override;
	private:
		float m_Angle{}; //radians
		float m_Radius{};
		float m_RotSpeed{}; //radians per sec
	};
}