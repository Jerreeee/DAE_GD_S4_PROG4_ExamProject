#include "SDL.h"
#undef main
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/FontImporter.h"

#include "EngineSetup.h"
#include "Components/HealthComponent.h"
#include "JREngine/Rendering/TextRendererComponent.h"
#include "SceneBuilders/UIBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	UIBuilder::UIBuilder(JRE::Scene& scene) : m_Scene{ scene }
	{
	}
	UIBuilder& UIBuilder::SetPlayer1(const JRE::GameObject& gameObject)
	{
		m_pPlayer1 = &gameObject;
		return *this;
	}
	void UIBuilder::Build()
	{
		assert(m_pPlayer1 && "UIBuilder::SetGameObjectWithHealthToTrack() | Must be called");

		auto handle = AssetImporterRegistry::GetInstance().ImportAsset(TextureImporter("HUD/Health_P1.png"));
		auto sprite = CreateAssetRef<Sprite>(SoftAssetRef<Texture2D>(handle));

		auto p1HealthCmp = m_pPlayer1->GetComponent<HealthComponent>();
		auto healthUIGO = std::make_unique<GameObject>("HealthUI");
		healthUIGO->m_PersistenceScope = PersistenceMask::PlayerAndUI;
		auto p1HealthUICmp = healthUIGO->AddComponent<HealthUIComponent>(*p1HealthCmp);
		p1HealthUICmp->SetSprite(SoftAssetRef<Sprite>(sprite));
		healthUIGO->SetWorldPosition(0.f, 648.f);
		m_Scene.Add(std::move(healthUIGO));

		//load font
		auto fontImporter = JRE::FontImporter("Fonts/Pixel_NES.otf");
		fontImporter.SetSize(30);
		auto fontHandle = JRE::AssetImporterRegistry::GetInstance().ImportAsset(std::move(fontImporter));
		auto fontSoftRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

		auto score = std::make_unique<GameObject>("Score");
		score->AddComponent<TextRendererComponent>("Score: 0", fontSoftRef, SDL_Color(255, 0, 0, 255));
		m_Scene.Add(std::move(score));
	}
}
