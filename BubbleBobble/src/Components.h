#pragma once
#include <memory>
#include "JREngine/ComponentBase.h"
#include "JREngine/Observer.h"
#include "Events.h"

namespace JREngine
{
	class Font;
	class GameObject;
	class TextRendererComponent;
}

namespace BubbleBobble
{
	class HealthComponent final : public JREngine::ComponentBase
	{
	public:
		HealthComponent(JREngine::GameObject& gameObject, int maxHealth);
		~HealthComponent();

		virtual void Update() override {};

		void SetHealth(int health);
		void SetMaxHealth(int maxHealth);
		void TakeDamage(int amount);
		int GetHealth() const { return m_Health; };

		JREngine::Observable& OnDamageEvent() { return *(m_DamageEvent.get()); };
	private:
		int m_Health{};
		int m_MaxHealth{};
		JREngine::Event_t m_DamageEvent{};
	};

	class ScoreComponent final : public JREngine::ComponentBase
	{
	public:
		ScoreComponent(JREngine::GameObject& gameObject);
		void IncreaseScore(int points);
		int GetScore() const { return m_Score; };

		virtual void Update() override {};
		JREngine::Observable& OnIncreasedScoreEvent() { return *(m_IncreasedScoreEvent.get()); };
	private:
		int m_Score{};
		JREngine::Event_t m_IncreasedScoreEvent{};
	};

	class PlayerUIComponent final : public JREngine::ComponentBase, public JREngine::IObserver
	{
	public:
		PlayerUIComponent(JREngine::GameObject& UIGameObject, JREngine::GameObject& player, std::shared_ptr<JREngine::Font> pFont);
		~PlayerUIComponent();
		virtual void Update() override {};
		virtual void OnNotify(JREngine::EventInfo& event) override;
	private:
		void SetTextComponentText(JREngine::TextRendererComponent* pTextComp, const std::string& prefix, const std::string& text);

		HealthComponent* m_pHealthComponent{};
		ScoreComponent* m_pScoreComponent{};
		JREngine::TextRendererComponent* m_pLivesText{}; 
		JREngine::TextRendererComponent* m_pScoreText{};
	};
}