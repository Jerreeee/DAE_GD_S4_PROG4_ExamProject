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

	void Event::AddObserver(IObserver* pObserver) const
	{
		if (!m_Observable) //lazy heap allocation
			m_Observable = std::make_unique<Observable>();
		m_Observable->AddObserver(pObserver);
	}
	void Event::RemoveObserver(IObserver* pObserver) const
	{
		if (m_Observable)
			m_Observable->RemoveObserver(pObserver);
	}
	void Event::Notify(EventInfo& event) const
	{
		if (m_Observable)
			m_Observable->NotifyObservers(event);
	}
}
