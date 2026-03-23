#include <algorithm>
#include "JREngine/Core/Event.h"
#include "Core/Observer.h"

namespace JRE
{
    void Observable::AddObserver(EventConnection* conn) const
    {
        m_Observers.push_back(conn);
    }

    void Observable::RemoveObserver(EventConnection* conn) const
    {
        auto it = std::find(m_Observers.begin(), m_Observers.end(), conn);
        if (it != m_Observers.end())
            m_Observers.erase(it);
    }

    void Observable::NotifyObservers(EventInfo& event) const
    {
        auto copy = m_Observers;
        for (auto* conn : copy)
            conn->callback(event);
    }

    void Observable::InvalidateAndClear() const
    {
        for (auto* conn : m_Observers)
            conn->event = nullptr;
        m_Observers.clear();
    }
}
