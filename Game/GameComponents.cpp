#include <sstream>
#include "GameComponents.h"
#include "GameEvents.h"
#include "../Minigin/GameObject.h"
#include "../Minigin/TextRendererComponent.h"
//#include "../Minigin/Observer.h"

namespace Game
{
	HealthComponent::HealthComponent(Engine::GameObject& gameObject, int maxHealth) :
		Engine::ComponentBase(gameObject),
		m_Health{ maxHealth },
		m_MaxHealth{ maxHealth },
		m_DamageEvent{ std::make_unique<Engine::Observable>() }
	{
	}
	HealthComponent::~HealthComponent() = default;
	void HealthComponent::SetHealth(int health)
	{
		if (health < 0 || health > m_MaxHealth)
			return;
		m_Health = health;
	}
	void HealthComponent::SetMaxHealth(int maxHealth)
	{
		m_MaxHealth = maxHealth;
	}
	void HealthComponent::TakeDamage(int amount)
	{
		m_Health -= amount;
		Engine::Event e(Engine::HashEventID("PlayerDamaged"));
		e.args = std::make_unique<PlayerDamagedEventArgs>(amount, m_Health);
		m_DamageEvent->NotifyObservers(e);
	}
	LivesComponentUI::LivesComponentUI(Engine::GameObject& gameObject, HealthComponent* pHealthComponent, std::shared_ptr<Engine::Font> pFont) :
		ComponentBase(gameObject),
		m_pHealthComponent{ pHealthComponent }
	{
		m_pHealthComponent->DamageEvent().AddObserver(this);
		m_pTextRendererComponent = gameObject.AddComponent<Engine::TextRendererComponent>("Lives: 3", pFont);
	}

	void LivesComponentUI::Notify(const Engine::Event& event)
	{
		switch (event.id)
		{
		case Engine::HashEventID("PlayerDamaged"):
		{
			std::stringstream ss{};
			auto args = static_cast<PlayerDamagedEventArgs*>(event.args.get());
			ss << "Lives: " << std::to_string(args->newhealth);
			m_pTextRendererComponent->SetText(ss.str());
			break;
		}
		}
	}
}