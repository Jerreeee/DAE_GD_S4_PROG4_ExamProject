#pragma once
#include <vector>

namespace JRE
{
    class GameObject;
    class EventInfo;
    struct EventConnection;

    class IObserver
    {
    public:
        virtual ~IObserver() = default;
        virtual void OnNotify(EventInfo& event) = 0;
    };

    struct ObserverEntry
    {
        IObserver* observer;
        EventConnection* connection;
    };

    class Observable final
    {
    public:
        void AddObserver(IObserver* pObserver, EventConnection* connection) const;
        EventConnection* RemoveObserver(IObserver* pObserver) const;
        void NotifyObservers(EventInfo& event) const;
        void Clear() const;

    private:
        mutable std::vector<ObserverEntry> m_Observers{};
    };
}
