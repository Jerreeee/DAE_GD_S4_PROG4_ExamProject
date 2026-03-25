#pragma once
#include <vector>

namespace JRE
{
    class EventInfo;
    struct EventConnection;

    class Observable final
    {
    public:
        void AddObserver(EventConnection* conn) const;
        void RemoveObserver(EventConnection* conn) const;
        void NotifyObservers(EventInfo& event) const;
        void InvalidateAndClear() const;

    private:
        mutable std::vector<EventConnection*> m_Observers{};
    };
}
