#pragma once
#include <vector>
#include <algorithm>
#include "JREngine/Core/Event.h"

namespace JRE
{
	class GameObject;
	class Observable;
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void OnNotify(EventInfo& event) = 0;
	};

	class Observable final
	{
	public:
		void AddObserver(IObserver* pObserver);
		void RemoveObserver(IObserver* pObserver);
		void NotifyObservers(EventInfo& event);
	private:
		std::vector<IObserver*> m_Observers{};
	};

	using Event_t = std::unique_ptr<Observable>;
}