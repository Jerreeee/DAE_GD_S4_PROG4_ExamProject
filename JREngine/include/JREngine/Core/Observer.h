#pragma once
#include <vector>

namespace JRE
{
	class GameObject;
	class Observable;
	class EventInfo;

	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void OnNotify(EventInfo& event) = 0;
	};

	class Observable final
	{
	public:
		void AddObserver(IObserver* pObserver) const;
		void RemoveObserver(IObserver* pObserver) const;
		void NotifyObservers(EventInfo& event) const;
	private:
		mutable std::vector<IObserver*> m_Observers{};
	};
}
