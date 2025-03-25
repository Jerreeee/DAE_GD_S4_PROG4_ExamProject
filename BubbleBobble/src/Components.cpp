#include <sstream>
#include "JREngine/GameObject.h"
#include "JREngine/TextRendererComponent.h"
#include "JREngine/SceneManager.h"
#include "JREngine/Scene.h"

#include "Components.h"
#include "Events.h"

namespace BubbleBobble
{
	HealthComponent::HealthComponent(JREngine::GameObject& gameObject, int maxHealth) :
		JREngine::ComponentBase(gameObject),
		m_Health{ maxHealth },
		m_MaxHealth{ maxHealth },
		m_DamageEvent{ std::make_unique<JREngine::Observable>() }
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
		JREngine::EventInfo e{ JREngine::CreateEvent<Event::PlayerDamaged>(amount, m_Health) };
		m_DamageEvent->NotifyObservers(e);
	}
	PlayerUIComponent::PlayerUIComponent(JREngine::GameObject& UIGameObject, JREngine::GameObject& player, std::shared_ptr<JREngine::Font> pFont) :
		ComponentBase(UIGameObject),
		m_pHealthComponent{ player.GetComponent<HealthComponent>() },
		m_pScoreComponent{ player.GetComponent<ScoreComponent>() }
	{
		m_pHealthComponent->OnDamageEvent().AddObserver(this);
		auto livesObject = std::make_unique<JREngine::GameObject>();
		m_pLivesText = livesObject->AddComponent<JREngine::TextRendererComponent>("", pFont);

		m_pScoreComponent->OnIncreasedScoreEvent().AddObserver(this);
		auto scoreObject = std::make_unique<JREngine::GameObject>();
		m_pScoreText = scoreObject->AddComponent<JREngine::TextRendererComponent>("", pFont);

		scoreObject->SetLocalPosition(0.f, 25.f);
		livesObject->SetParent(&UIGameObject);
		scoreObject->SetParent(&UIGameObject);

		auto& scene = JREngine::SceneManager::GetInstance().GetCurrentScene();
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

	void PlayerUIComponent::OnNotify(JREngine::EventInfo& event)
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
	void PlayerUIComponent::SetTextComponentText(JREngine::TextRendererComponent* pTextComp, const std::string& prefix, const std::string& text)
	{
		std::stringstream ss{};
		ss << prefix << text;
		pTextComp->SetText(ss.str());
	}
	ScoreComponent::ScoreComponent(JREngine::GameObject& gameObject) :
		ComponentBase(gameObject),
		m_IncreasedScoreEvent{ std::make_unique<JREngine::Observable>() }
	{
	}
	void ScoreComponent::IncreaseScore(int points)
	{
		m_Score += points;
		JREngine::EventInfo e{ JREngine::CreateEvent<Event::IncreasedScore>(m_Score) };
		m_IncreasedScoreEvent->NotifyObservers(e);
	}
}