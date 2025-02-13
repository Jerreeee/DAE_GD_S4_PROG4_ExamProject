#pragma once
#include "Singleton.h"

namespace dae
{
	class Time : public Singleton<Time>
	{
	public:
		float GetFixedTimeStep() { return GetInstance().m_FixedTimeStep; }
		float GetDeltaTime() { return GetInstance().m_DeltaTime; }
		void SetFixedTimeStep(float fixedTimeStep);
		void SetDelaTime(float deltaTime);
	private:
		friend class Singleton<Time>;
		Time() = default;

		float m_FixedTimeStep{};
		float m_DeltaTime{};
	};
}

