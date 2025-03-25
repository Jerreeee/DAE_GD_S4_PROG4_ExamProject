#pragma once
#include "Singleton.h"

namespace Engine
{
	class Timer : public Singleton<Timer>
	{
	public:
		float GetFixedTimeStep() { return GetInstance().m_FixedTimeStep; }
		float GetDeltaTime() { return GetInstance().m_DeltaTime; }
		void SetFixedTimeStep(float fixedTimeStep);
		void SetDeltaTime(float deltaTime);
	private:
		friend class Singleton<Timer>;
		Timer() = default;

		float m_FixedTimeStep{};
		float m_DeltaTime{};
	};
}

