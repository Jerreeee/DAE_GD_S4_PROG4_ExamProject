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
		m_MaxHealth{ maxHealth }
	{
	}
	HealthComponent::~HealthComponent() = default;

	void HealthComponent::SetHitSound(const JRE::SoftAssetRef<JRE::ISoundClip>& hitSound)
	{
		m_HitSound = hitSound;
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
		//JRE::EventInfo e{ JRE::CreateEvent<Event::PlayerDamaged>(amount, m_Health) };
		//OnDamageEvent.Notify(e);

		if (!m_HitSound.IsLoaded())
			m_HitSound.Get();

		JRE::ServiceLocator::GetSoundSystem().Play(m_HitSound.Get());
	}
}
