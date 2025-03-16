#pragma once
#include <memory>
#include "../Minigin/ComponentBase.h"
#include "../Minigin/Observer.h"
#include "GameEvents.h"

namespace Engine
{
	class Font;
	class GameObject;
	class TextRendererComponent;
}

namespace Game
{
	class HealthComponent final : public Engine::ComponentBase
	{
	public:
		HealthComponent(Engine::GameObject& gameObject, int maxHealth);
		~HealthComponent();

		virtual void Update() override {};

		void SetHealth(int health);
		void SetMaxHealth(int maxHealth);
		void TakeDamage(int amount);
		int GetHealth() const { return m_Health; };

		Engine::Observable& DamageEvent() { return *(m_DamageEvent.get()); };
	private:
		int m_Health{};
		int m_MaxHealth{};
		std::unique_ptr<Engine::Observable> m_DamageEvent{};
	};

	class ScoreComponent final : public Engine::ComponentBase
	{
	public:
		ScoreComponent(Engine::GameObject& gameObject);
		void IncreaseScore(int points);
		int GetScore() const { return m_Score; };

		virtual void Update() override {};
		Engine::Observable& IncreasedScoreEvent() { return *(m_IncreasedScoreEvent.get()); };
	private:
		int m_Score{};
		std::unique_ptr<Engine::Observable> m_IncreasedScoreEvent{};
	};

	class PlayerUIComponent final : public Engine::ComponentBase, public Engine::Observer
	{
	public:
		PlayerUIComponent(Engine::GameObject& UIGameObject, Engine::GameObject& player, std::shared_ptr<Engine::Font> pFont);
		virtual void Update() override {};
		virtual void Notify(const Engine::Event& event) override;
	private:
		void SetTextComponentText(Engine::TextRendererComponent* pTextComp, const std::string& prefix, const std::string& text);

		HealthComponent* m_pHealthComponent{};
		ScoreComponent* m_pScoreComponent{};
		Engine::TextRendererComponent* m_pLivesText{}; 
		Engine::TextRendererComponent* m_pScoreText{};
	};
}