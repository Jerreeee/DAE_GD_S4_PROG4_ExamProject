#include <iostream>
#include "Core/Observer.h"

namespace JRE
{
	void Observable::AddObserver(IObserver* pObserver)
	{
		if (std::find(m_Observers.begin(), m_Observers.end(), pObserver) == m_Observers.end())
			m_Observers.emplace_back(pObserver);
	}
	void Observable::RemoveObserver(IObserver* pObserver)
	{
		m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver), m_Observers.end());
	}
	void Observable::NotifyObservers(EventInfo& event)
	{
		for (const auto& pObserver : m_Observers)
			pObserver->OnNotify(event);
	}
}