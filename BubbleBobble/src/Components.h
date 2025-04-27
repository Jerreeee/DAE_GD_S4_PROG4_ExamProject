#pragma once
#include <memory>
#include "JREngine/ComponentBase.h"
#include "JREngine/Observer.h"
#include "JREngine/ResourceHandle.h"
#include "Events.h"

namespace JRE
{
	class Font;
	class GameObject;
	class TextRendererComponent;
}

namespace BubbleBobble
{
	class HealthComponent final : public JRE::ComponentBase
	{
	public:
		HealthComponent(JRE::GameObject& gameObject, int maxHealth);
		~HealthComponent();

		virtual void Update() override {};

		void SetHealth(int health);
		void SetMaxHealth(int maxHealth);
		void TakeDamage(int amount);
		int GetHealth() const { return m_Health; };

		JRE::Observable& OnDamageEvent() { return *(m_DamageEvent.get()); };
	private:
		int m_Health{};
		int m_MaxHealth{};
		JRE::Event_t m_DamageEvent{};
	};

	class ScoreComponent final : public JRE::ComponentBase
	{
	public:
		ScoreComponent(JRE::GameObject& gameObject);
		void IncreaseScore(int points);
		int GetScore() const { return m_Score; };

		virtual void Update() override {};
		JRE::Observable& OnIncreasedScoreEvent() { return *(m_IncreasedScoreEvent.get()); };
	private:
		int m_Score{};
		JRE::Event_t m_IncreasedScoreEvent{};
	};

	class PlayerUIComponent final : public JRE::ComponentBase, public JRE::IObserver
	{
	public:
		PlayerUIComponent(JRE::GameObject& UIGameObject, JRE::GameObject& player, JRE::ResourceHandle<JRE::Font> fontHandle);
		~PlayerUIComponent();
		virtual void Update() override {};
		virtual void OnNotify(JRE::EventInfo& event) override;
	private:
		void SetTextComponentText(JRE::TextRendererComponent* pTextComp, const std::string& prefix, const std::string& text);

		HealthComponent* m_pHealthComponent{};
		ScoreComponent* m_pScoreComponent{};
		JRE::TextRendererComponent* m_pLivesText{};
		JRE::TextRendererComponent* m_pScoreText{};
	};
}