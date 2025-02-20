#include "Timer.h"

namespace dae
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
