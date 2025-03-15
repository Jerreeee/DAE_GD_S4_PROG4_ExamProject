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

	class LivesComponentUI final : public Engine::ComponentBase, public Engine::Observer
	{
	public:
		LivesComponentUI(Engine::GameObject& gameObject, HealthComponent* pHealthComponent, std::shared_ptr<Engine::Font> pFont);
		virtual void Update() override {};
		virtual void Notify(const Engine::Event& event) override;
	private:
		HealthComponent* m_pHealthComponent{};
		Engine::TextRendererComponent* m_pTextRendererComponent{};
	};
}