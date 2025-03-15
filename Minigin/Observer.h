#pragma once
#include <vector>
#include <algorithm>
#include "Event.h"

namespace Engine
{
	class GameObject;
	class Observable;
	class Observer
	{
	public:
		virtual ~Observer();
		virtual void Notify(const Event& event) = 0;
		void AddObservable(Observable* pObservable);
		void RemoveObservable(Observable* pObservable);
	private:
		std::vector<Observable*> m_Observables{};
	};

	class Observable
	{
	public:
		virtual ~Observable();
		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void NotifyObservers(const Event& event);
	private:
		std::vector<Observer*> m_Observers{};
	};
}