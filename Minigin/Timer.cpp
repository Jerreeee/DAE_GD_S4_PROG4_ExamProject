#include "Timer.h"

namespace dae
{
	void Timer::SetFixedTimeStep(float fixedTimeStep)
	{
		m_FixedTimeStep = fixedTimeStep;
	}

	void Timer::SetDelaTime(float deltaTime)
	{
		m_DeltaTime = deltaTime;
	}
}
