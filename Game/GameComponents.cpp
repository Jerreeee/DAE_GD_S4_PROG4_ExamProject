#include <sstream>
#include "GameComponents.h"
#include "GameEvents.h"
#include "../Minigin/GameObject.h"
#include "../Minigin/TextRendererComponent.h"
#include "../Minigin/SceneManager.h"
#include "../Minigin/Scene.h"

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
		Engine::Event e(Engine::HashEventID("Damaged"));
		e.args = std::move(std::make_unique<DamagedEventArgs>(amount, m_Health));
		m_DamageEvent->NotifyObservers(e);
	}
	PlayerUIComponent::PlayerUIComponent(Engine::GameObject& UIGameObject, Engine::GameObject& player, std::shared_ptr<Engine::Font> pFont) :
		ComponentBase(UIGameObject),
		m_pHealthComponent{ player.GetComponent<HealthComponent>() },
		m_pScoreComponent{ player.GetComponent<ScoreComponent>() }
	{
		m_pHealthComponent->DamageEvent().AddObserver(this);
		auto livesObject = std::make_unique<Engine::GameObject>();
		m_pLivesText = livesObject->AddComponent<Engine::TextRendererComponent>("", pFont);
		m_pScoreComponent->IncreasedScoreEvent().AddObserver(this);
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

	void PlayerUIComponent::Notify(const Engine::Event& event)
	{
		switch (event.id)
		{
		case Engine::HashEventID("Damaged"):
		{
			auto args = static_cast<DamagedEventArgs*>(event.args.get());
			SetTextComponentText(m_pLivesText, "Lives: ", std::to_string(args->newhealth));
			break;
		}
		case Engine::HashEventID("IncreasedScore"):
		{
			auto args = static_cast<IncreasedScoreEventArgs*>(event.args.get());
			SetTextComponentText(m_pScoreText, "Score: ", std::to_string(args->newScore));
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
		Engine::Event e(Engine::HashEventID("IncreasedScore"));
		e.args = std::move(std::make_unique<IncreasedScoreEventArgs>(m_Score));
		m_IncreasedScoreEvent->NotifyObservers(e);
	}
}