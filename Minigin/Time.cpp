#include "Time.h"

namespace dae
{
	void Time::SetFixedTimeStep(float fixedTimeStep)
	{
		m_FixedTimeStep = fixedTimeStep;
	}

	void Time::SetDelaTime(float deltaTime)
	{
		m_DeltaTime = deltaTime;
	}
}
