#pragma once
#include "JREngine/Rendering/RendererComponentBase.h"
#include "JREngine/Core/Event.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Sprite.h"

namespace JRE
{
	class GameObject;
	class Sprite;
}

namespace BubbleBobble
{
	namespace Events
	{
		struct HealthChanged
		{
			static const JRE::EventID ID{ JRE::HashEventID("HealthChanged") };
			struct Args : public JRE::EventArgs
			{
				Args(int _amount, int _newHealth)
					: amount{ _amount }, newHealth{ _newHealth }
				{
				}
				int amount;
				int newHealth;
			};
		};
	}

	class HealthComponent final : public JRE::ComponentBase
	{
	public:
		HealthComponent(JRE::GameObject& gameObjeect);

		virtual void Update() override {};

		void SetHealth(int health);
		void SetMaxHealth(int maxHealth);
		void TakeDamage(int amount);
		int GetHealth() const { return m_Health; };

		JRE::Event OnHealthChanged{};
	private:
		int m_Health{};
		int m_MaxHealth{};
	};

	class HealthUIComponent final : public JRE::RendererComponentBase, public JRE::IObserver
	{
	public:
		HealthUIComponent(JRE::GameObject& gameObject, const HealthComponent& healthCmp);

		virtual void Render() const override;
		virtual void OnNotify(JRE::EventInfo& event) override;

		void SetSprite(JRE::SoftAssetRef<JRE::Sprite> sprite);
	private:
		const HealthComponent* m_pHealthCmp{ nullptr };
		int m_Nr{ 1 };
		JRE::SoftAssetRef<JRE::Sprite> m_Sprite{ nullptr };
	};
}
