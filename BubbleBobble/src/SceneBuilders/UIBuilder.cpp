#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"

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

		auto handle = AssetImporter::GetInstance().ImportAsset(TextureImporter("HUD/Health_P1.png"));
		auto sprite = CreateAssetRef<Sprite>(SoftAssetRef<Texture2D>(handle));

		auto p1HealthCmp = m_pPlayer1->GetComponent<HealthComponent>();
		auto healthUIGO = std::make_unique<GameObject>("HealthUI");
		healthUIGO->m_PersistenceScope = PersistenceMask::PlayerAndUI;
		auto p1HealthUICmp = healthUIGO->AddComponent<HealthUIComponent>(*p1HealthCmp);
		p1HealthUICmp->SetSprite(SoftAssetRef<Sprite>(sprite));
		healthUIGO->SetWorldPosition(0.f, 648.f);
		m_Scene.Add(std::move(healthUIGO));
	}
}
