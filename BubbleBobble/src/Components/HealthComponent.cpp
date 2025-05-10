#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Audio/ISoundSystem.h"
#include "Events.h"
#include "Components/HealthComponent.h"

namespace BubbleBobble
{
	HealthComponent::HealthComponent(JRE::GameObject& gameObject, int maxHealth) :
		JRE::ComponentBase(gameObject),
		m_Health{ maxHealth },
		m_MaxHealth{ maxHealth },
		m_DamageEvent{ std::make_unique<JRE::Observable>() }
	{
	}
	HealthComponent::~HealthComponent() = default;

	void HealthComponent::SetHitSound(JRE::AssetHandle hitSoundHandle)
	{
		m_HitSoundHandle = hitSoundHandle;
	}
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
		JRE::EventInfo e{ JRE::CreateEvent<Event::PlayerDamaged>(amount, m_Health) };
		m_DamageEvent->NotifyObservers(e);

		if (!m_pHitSound)
			m_pHitSound = JRE::ResourceManager::TryGetAsset<JRE::ISoundClip>(m_HitSoundHandle);

		JRE::ServiceLocator::GetSoundSystem().Play(m_pHitSound);
	}
}
