#include "Timer.h"

namespace JRE
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
