#pragma once
#include "JREngine/ComponentBase.h"

namespace JREngine
{
	class GameObject;
}

namespace BubbleBobble
{
	class RotateParentComponent final: public JREngine::ComponentBase
	{
	public:
		RotateParentComponent(JREngine::GameObject& gameObject, float radius, float rotSpeedDeg);
		
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