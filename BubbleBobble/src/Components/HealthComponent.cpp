#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Audio/ISoundSystem.h"
#include "JREngine/Rendering/SDLRenderer.h"

#include "Components/HealthComponent.h"

using namespace JRE;

namespace BubbleBobble
{
	HealthComponent::HealthComponent(JRE::GameObject& gameObjeect)
		: ComponentBase(gameObjeect)
	{
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
		if (m_Health <= 0) return;

		m_Health -= amount;
		JRE::EventInfo e{ JRE::CreateEvent<Events::HealthChanged>(amount, m_Health) };
		OnHealthChanged.Notify(e);
	}

	HealthUIComponent::HealthUIComponent(JRE::GameObject& gameObject, const HealthComponent& healthCmp)
		: RendererComponentBase(gameObject)
		, m_pHealthCmp{ &healthCmp }
		, m_Nr{ m_pHealthCmp->GetHealth() }
	{
		m_pHealthCmp->OnHealthChanged.AddObserver(this);
	}
	void HealthUIComponent::Render() const
	{
		if (!m_Sprite) return;

		const Region& region = m_Sprite->GetSrcRegion();
		glm::vec2 pos = glm::vec2(GetGameObject().GetWorldPosition());
		for (int i{}; i < m_Nr; ++i)
		{
			float x = pos.x + i * region.width;
			SDLRenderer::GetInstance().RenderSprite(m_Sprite.Get(), x, pos.y);
		}
	}
	void HealthUIComponent::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case Events::HealthChanged::ID:
		{
			auto& args = event.GetArgs<Events::HealthChanged>();
			m_Nr = args.newHealth;
			break;
		}
		}
	}
	void HealthUIComponent::SetSprite(JRE::SoftAssetRef<JRE::Sprite> sprite)
	{
		m_Sprite = sprite;
	}
}
