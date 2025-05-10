#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Observer.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Audio/ISoundClip.h"

namespace JRE
{
	class GameObject;
	class ISoundClip;
}

namespace BubbleBobble
{
	class HealthComponent final : public JRE::ComponentBase
	{
	public:
		HealthComponent(JRE::GameObject& gameObject, int maxHealth);
		~HealthComponent();

		virtual void Update() override {};

		void SetHitSound(const JRE::SoftAssetRef<JRE::ISoundClip>& hitSound);
		void SetHealth(int health);
		void SetMaxHealth(int maxHealth);
		void TakeDamage(int amount);
		int GetHealth() const { return m_Health; };

		JRE::Observable& OnDamageEvent() { return *(m_DamageEvent.get()); };
	private:
		int m_Health{};
		int m_MaxHealth{};
		JRE::Event_t m_DamageEvent{};
		JRE::SoftAssetRef<JRE::ISoundClip> m_HitSound{};
	};
}
