#include "Timer.h"

namespace Engine
{
	void Timer::SetFixedTimeStep(float fixedTimeStep)
	{
		m_FixedTimeStep = fixedTimeStep;
	}

	void Timer::SetDeltaTime(float deltaTime)
	{
		m_DeltaTime = deltaTime;
	}
}
