#include <algorithm>
#include "JREngine/Core/Event.h"
#include "Core/Observer.h"

namespace JRE
{
	void Observable::AddObserver(IObserver* pObserver) const
	{
		if (std::find(m_Observers.begin(), m_Observers.end(), pObserver) == m_Observers.end())
			m_Observers.emplace_back(pObserver);
	}
	void Observable::RemoveObserver(IObserver* pObserver) const
	{
		m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver), m_Observers.end());
	}
	void Observable::NotifyObservers(EventInfo& event) const
	{
		for (const auto& pObserver : m_Observers)
			if (pObserver)
				pObserver->OnNotify(event);
	}
}
