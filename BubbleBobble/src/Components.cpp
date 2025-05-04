#include <sstream>
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Rendering/TextRendererComponent.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Resources/ResourceManager.h"
#include "JREngine/Audio/ISoundSystem.h"
#include "JREngine/Scene/Scene.h"

#include "Components.h"
#include "Events.h"

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
		{
			m_pHitSound = JRE::ResourceManager::GetAsset<JRE::ISoundClip>(m_HitSoundHandle);
			if (!m_pHitSound) return;
		}
		JRE::ServiceLocator::GetSoundSystem().Play(m_pHitSound);
	}
	PlayerUIComponent::PlayerUIComponent(JRE::GameObject& UIGameObject, JRE::GameObject& player, JRE::AssetHandle fontHandle) :
		ComponentBase(UIGameObject),
		m_pHealthComponent{ player.GetComponent<HealthComponent>() },
		m_pScoreComponent{ player.GetComponent<ScoreComponent>() }
	{
		m_pHealthComponent->OnDamageEvent().AddObserver(this);
		auto livesObject = std::make_unique<JRE::GameObject>();
		m_pLivesText = livesObject->AddComponent<JRE::TextRendererComponent>("", fontHandle);

		m_pScoreComponent->OnIncreasedScoreEvent().AddObserver(this);
		auto scoreObject = std::make_unique<JRE::GameObject>();
		m_pScoreText = scoreObject->AddComponent<JRE::TextRendererComponent>("", fontHandle);

		scoreObject->SetLocalPosition(0.f, 25.f);
		livesObject->SetParent(&UIGameObject);
		scoreObject->SetParent(&UIGameObject);

		auto& scene = JRE::SceneManager::GetInstance().GetCurrentScene();
		scene.Add(std::move(livesObject));
		scene.Add(std::move(scoreObject));

		SetTextComponentText(m_pLivesText, "Lives: ", std::to_string(m_pHealthComponent->GetHealth()));
		SetTextComponentText(m_pScoreText, "Score: ", std::to_string(0));
	}

	PlayerUIComponent::~PlayerUIComponent()
	{
		m_pHealthComponent->OnDamageEvent().RemoveObserver(this);
		m_pScoreComponent->OnIncreasedScoreEvent().RemoveObserver(this);
	}

	void PlayerUIComponent::OnNotify(JRE::EventInfo& event)
	{
		switch (event.GetID())
		{
		case Event::PlayerDamaged::ID:
		{
			auto& args = event.GetArgs<Event::PlayerDamaged>();
			SetTextComponentText(m_pLivesText, "Lives: ", std::to_string(args.newhealth));
			break;
		}
		case Event::IncreasedScore::ID:
		{
			auto& args = event.GetArgs<Event::IncreasedScore>();
			SetTextComponentText(m_pScoreText, "Score: ", std::to_string(args.newScore));
		}
		}
	}
	void PlayerUIComponent::SetTextComponentText(JRE::TextRendererComponent* pTextComp, const std::string& prefix, const std::string& text)
	{
		std::stringstream ss{};
		ss << prefix << text;
		pTextComp->SetText(ss.str());
	}
	ScoreComponent::ScoreComponent(JRE::GameObject& gameObject) :
		ComponentBase(gameObject),
		m_IncreasedScoreEvent{ std::make_unique<JRE::Observable>() }
	{
	}
	void ScoreComponent::IncreaseScore(int points)
	{
		m_Score += points;
		JRE::EventInfo e{ JRE::CreateEvent<Event::IncreasedScore>(m_Score) };
		m_IncreasedScoreEvent->NotifyObservers(e);
	}
}