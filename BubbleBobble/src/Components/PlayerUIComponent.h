#pragma once
#include <memory>
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Core/Observer.h"
#include "Events.h"

namespace JRE
{
	class GameObject;
	class TextRendererComponent;
}

namespace BubbleBobble
{
	class HealthComponent;
	class ScoreComponent;
	class PlayerUIComponent final : public JRE::ComponentBase, public JRE::IObserver
	{
	public:
		PlayerUIComponent(JRE::GameObject& UIGameObject, JRE::GameObject& player, const JRE::SoftAssetRef<JRE::Font>& softFontRef);
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
