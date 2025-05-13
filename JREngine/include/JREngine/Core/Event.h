#pragma once
#include <array>
#include <memory>
#include <concepts>
#include <cassert>
#include "JREngine/Core/Observer.h"

namespace JRE
{
	class Event final
	{
	public:
		void AddObserver(IObserver* pObserver)
		{
			if (!m_Observable) //lazy heap allocation
				m_Observable = std::make_unique<Observable>();
			m_Observable->AddObserver(pObserver);
		}
		void RemoveObserver(IObserver* pObserver)
		{
			if (m_Observable)
				m_Observable->RemoveObserver(pObserver);
		}
		void Notify(EventInfo& event)
		{
			if (m_Observable)
				m_Observable->NotifyObservers(event);
		}
	private:
		std::unique_ptr<Observable> m_Observable{ nullptr };
	};
}
