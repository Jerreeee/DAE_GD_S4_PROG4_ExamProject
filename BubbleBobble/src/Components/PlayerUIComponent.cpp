#include "SDL.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"
#include "JREngine/Rendering/TextRendererComponent.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/Font.h"

#include "Events.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/PlayerUIComponent.h"

namespace BubbleBobble
{
	PlayerUIComponent::PlayerUIComponent(JRE::GameObject& UIGameObject, JRE::GameObject& player, const JRE::SoftAssetRef<JRE::Font>& softFontRef) :
		ComponentBase(UIGameObject),
		m_pHealthComponent{ player.GetComponent<HealthComponent>() },
		m_pScoreComponent{ player.GetComponent<ScoreComponent>() }
	{
		SDL_Color color = SDL_Color{ 255, 255, 255, 255 };
		m_pHealthComponent->OnDamageEvent.AddObserver(this);
		auto livesObject = std::make_unique<JRE::GameObject>();
		m_pLivesText = livesObject->AddComponent<JRE::TextRendererComponent>("", softFontRef, color);

		m_pScoreComponent->OnIncreasedScoreEvent.AddObserver(this);
		auto scoreObject = std::make_unique<JRE::GameObject>();
		m_pScoreText = scoreObject->AddComponent<JRE::TextRendererComponent>("", softFontRef, color);

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
		m_pHealthComponent->OnDamageEvent.RemoveObserver(this);
		m_pScoreComponent->OnIncreasedScoreEvent.RemoveObserver(this);
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
}