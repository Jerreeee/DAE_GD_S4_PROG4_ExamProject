#include <algorithm>
#include "JREngine/Core/Event.h"
#include "Core/Observer.h"

namespace JRE
{
    void Observable::AddObserver(IObserver* pObserver, EventConnection* connection) const
    {
        // Avoid duplicates
        for (auto& entry : m_Observers)
            if (entry.observer == pObserver)
                return;

        m_Observers.push_back({ pObserver, connection });
    }

    EventConnection* Observable::RemoveObserver(IObserver* pObserver) const
    {
        for (size_t i = 0; i < m_Observers.size(); ++i)
        {
            if (m_Observers[i].observer == pObserver)
            {
                EventConnection* conn = m_Observers[i].connection;

                m_Observers.erase(m_Observers.begin() + i);

                return conn;
            }
        }
        return nullptr;
    }

    void Observable::NotifyObservers(EventInfo& event) const
    {
        for (auto& entry : m_Observers)
            if (entry.observer)
                entry.observer->OnNotify(event);
    }

    void Observable::Clear() const
    {
        m_Observers.clear();
    }
}
