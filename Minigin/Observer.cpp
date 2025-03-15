#include "Observer.h"
#include <iostream>
namespace Engine
{
	Observable::~Observable()
	{
		for (auto& pObserver : m_Observers)
			pObserver->RemoveObservable(this);
	}
	void Observable::AddObserver(Observer* pObserver)
	{
		if (std::find(m_Observers.begin(), m_Observers.end(), pObserver) == m_Observers.end())
			m_Observers.emplace_back(pObserver);
		pObserver->AddObservable(this);
	}
	void Observable::RemoveObserver(Observer* pObserver)
	{
		m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver), m_Observers.end());
	}
	void Observable::NotifyObservers(const Event& event)
	{
		for (const auto& pObserver : m_Observers)
			pObserver->Notify(event);
	}
	Observer::~Observer()
	{
		for (auto& pObservable : m_Observables)
			pObservable->RemoveObserver(this);
	}
	void Observer::AddObservable(Observable* pObservable)
	{
		if (std::find(m_Observables.begin(), m_Observables.end(), pObservable) == m_Observables.end())
			m_Observables.emplace_back(pObservable);
	}
	void Observer::RemoveObservable(Observable* pObservable)
	{
		m_Observables.erase(std::remove(m_Observables.begin(), m_Observables.end(), pObservable), m_Observables.end());
	}
}