#include <iostream>

#include "Core/Event.h"


namespace JRE
{
	EventInfo::EventInfo(EventID id, std::unique_ptr<EventArgs> args)
		: m_ID(id), m_Args(std::move(args))
	{
	}
	EventInfo::EventInfo(EventInfo&& eventInfo) noexcept :
		m_ID{ eventInfo.m_ID }, m_Args{ std::move(eventInfo.m_Args) }
	{
	}
	EventInfo& EventInfo::operator=(EventInfo&& eventInfo) noexcept
	{
		if (this != &eventInfo)
		{
			m_ID = std::move(eventInfo.m_ID);
			m_Args = std::move(eventInfo.m_Args);
		}
		return *this;
	}
	Event::~Event()
	{
		// Invalidate all connections
		for (auto& connection : m_Connections)
			connection->active = false;

		m_Connections.clear();

		if (m_Observable)
			m_Observable->Clear();
	}
	std::shared_ptr<EventConnection> Event::AddObserver(IObserver* pObserver) const
	{
		if (!m_Observable)
			m_Observable = std::make_unique<Observable>();

		auto connection = std::make_shared<EventConnection>(this, true);
		m_Connections.emplace_back(connection);

		m_Observable->AddObserver(pObserver, connection.get());

		return m_Connections.back();
	}
	void Event::RemoveObserver(IObserver* pObserver) const
	{
		if (!m_Observable)
			return;

		EventConnection* connection = m_Observable->RemoveObserver(pObserver);
		if (!connection)
			return;

		// Remove the owned connection
		auto it = std::find_if(m_Connections.begin(), m_Connections.end(),
			[connection](auto& ptr) {
				return ptr.get() == connection;
			}
		);
		if (it != m_Connections.end())
			m_Connections.erase(it);
	}
	void Event::Notify(EventInfo& event) const
	{
		if (m_Observable)
			m_Observable->NotifyObservers(event);
	}
	void EventConnection::Disconnect(IObserver* observer) const
	{
		if (active && event)
			event->RemoveObserver(observer);
	}
}
