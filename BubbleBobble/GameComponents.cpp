#include <sstream>
#include "GameComponents.h"
#include "GameEvents.h"
#include "../Minigin/GameObject.h"
#include "../Minigin/TextRendererComponent.h"
#include "../Minigin/SceneManager.h"
#include "../Minigin/Scene.h"
#include "GameEvents.h"

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
		Engine::EventInfo e{ Engine::CreateEvent<Event::PlayerDamaged>(amount, m_Health) };
		m_DamageEvent->NotifyObservers(e);
	}
	PlayerUIComponent::PlayerUIComponent(Engine::GameObject& UIGameObject, Engine::GameObject& player, std::shared_ptr<Engine::Font> pFont) :
		ComponentBase(UIGameObject),
		m_pHealthComponent{ player.GetComponent<HealthComponent>() },
		m_pScoreComponent{ player.GetComponent<ScoreComponent>() }
	{
		m_pHealthComponent->OnDamageEvent().AddObserver(this);
		auto livesObject = std::make_unique<Engine::GameObject>();
		m_pLivesText = livesObject->AddComponent<Engine::TextRendererComponent>("", pFont);

		m_pScoreComponent->OnIncreasedScoreEvent().AddObserver(this);
		auto scoreObject = std::make_unique<Engine::GameObject>();
		m_pScoreText = scoreObject->AddComponent<Engine::TextRendererComponent>("", pFont);

		scoreObject->SetLocalPosition(0.f, 25.f);
		livesObject->SetParent(&UIGameObject);
		scoreObject->SetParent(&UIGameObject);

		auto& scene = Engine::SceneManager::GetInstance().GetCurrentScene();
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

	void PlayerUIComponent::OnNotify(Engine::EventInfo& event)
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
	void PlayerUIComponent::SetTextComponentText(Engine::TextRendererComponent* pTextComp, const std::string& prefix, const std::string& text)
	{
		std::stringstream ss{};
		ss << prefix << text;
		pTextComp->SetText(ss.str());
	}
	ScoreComponent::ScoreComponent(Engine::GameObject& gameObject) :
		ComponentBase(gameObject),
		m_IncreasedScoreEvent{ std::make_unique<Engine::Observable>() }
	{
	}
	void ScoreComponent::IncreaseScore(int points)
	{
		m_Score += points;
		Engine::EventInfo e{ Engine::CreateEvent<Event::IncreasedScore>(m_Score) };
		m_IncreasedScoreEvent->NotifyObservers(e);
	}
}