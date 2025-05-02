#include "Core/Event.h"

namespace JRE
{
	EventInfo::EventInfo(EventID id) : m_ID{ id } {}
	EventInfo::~EventInfo() = default;
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
}