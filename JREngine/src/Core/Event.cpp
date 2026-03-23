#include "Core/Event.h"

namespace JRE
{
	EventInfo::EventInfo(EventID id, std::unique_ptr<EventArgs> args)
		: m_ID(id), m_Args(std::move(args))
	{
	}

	Event::~Event()
	{
		if (m_Observable)
			m_Observable->InvalidateAndClear();
	}

	std::shared_ptr<EventConnection> Event::AddObserver(std::function<void(EventInfo&)> callback) const
	{
		if (!m_Observable)
			m_Observable = std::make_unique<Observable>();

		auto conn = std::make_shared<EventConnection>(this, std::move(callback));
		m_Observable->AddObserver(conn.get());
		return conn;
	}

	void Event::RemoveObserver(EventConnection* conn) const
	{
		if (m_Observable)
			m_Observable->RemoveObserver(conn);
	}

	void Event::Notify(EventInfo& event) const
	{
		if (m_Observable)
			m_Observable->NotifyObservers(event);
	}
}
