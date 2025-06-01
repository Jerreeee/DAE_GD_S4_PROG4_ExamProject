#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/FontImporter.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Rendering/TextRendererComponent.h"

#include "MenuBuilders.h"

namespace BubbleBobble
{
	void BuilderHelpers::AddCenteredTxt(JRE::Scene& scene, const std::string& text, JRE::SoftAssetRef<JRE::Font> font, float centerX, float y, SDL_Color color)
	{
		auto go = std::make_unique<JRE::GameObject>(text);
		auto cmp = go->AddComponent<JRE::TextRendererComponent>(text, font, color);
		const JRE::Region& region = cmp->GetSprite().GetSrcRegion();
		float x = centerX - region.width / 2;
		go->SetWorldPosition(x, y);
		scene.Add(std::move(go));
	}

	MainMenuBuilder::MainMenuBuilder(JRE::Scene& scene)
		: m_Scene{ scene }
	{
	}
	void MainMenuBuilder::Build()
	{
		auto logo = std::make_unique<JRE::GameObject>("Logo");
		logo->AddComponent<JRE::SpriteRendererComponent>();
		auto spriteAnimCmp = logo->AddComponent<JRE::SpriteAnimatorComponent>();
		auto logoHandle = JRE::AssetImporter::GetInstance().ImportAsset(JRE::TextureImporter("HUD/Logo_Anim.png"));
		auto logoRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(logoHandle);
		auto spritesRef = JRE::SpriteEditor::SplitTexture2D(logoRef, 6, 6, 1);
		std::vector<JRE::SoftAssetRef<JRE::Sprite>> spriteSoftRefs{};
		for (auto& spriteRef : spritesRef)
			spriteSoftRefs.emplace_back(JRE::SoftAssetRef<JRE::Sprite>(spriteRef));
		auto clipRef = JRE::CreateAssetRef<JRE::SpriteAnimationClip>(spriteSoftRefs, 2);
		spriteAnimCmp->SetSpriteAnimationClip(clipRef);
		//const JRE::Region& region = spriteRef->GetSrcRegion();
		logo->SetWorldPosition(117.f, 50.f);

		//load font
		auto fontImporter = JRE::FontImporter("Fonts/Pixel_NES.otf");
		fontImporter.SetSize(20);
		auto fontHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(fontImporter));
		auto fontSoftRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

		const int centerX = 384;
		SDL_Color color = { 255, 255, 255, 255 };
		BuilderHelpers::AddCenteredTxt(m_Scene, "<- FOR SINGLEPLAYER", fontSoftRef, centerX, 520.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "<- FOR MULTIPLAYER", fontSoftRef, centerX, 560.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "<- FOR VERSUS", fontSoftRef, centerX, 600.f, color);

		m_Scene.Add(std::move(logo));
	}

	LoadingMenuBuilder::LoadingMenuBuilder(JRE::Scene& scene)
		: m_Scene{ scene }
	{
	}
	void LoadingMenuBuilder::Build()
	{
		//load font
		auto fontImporter = JRE::FontImporter("Fonts/Pixel_NES.otf");
		fontImporter.SetSize(20);
		auto fontHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(fontImporter));
		auto fontSoftRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

		const int centerX = 384;
		SDL_Color color = { 255, 0, 0, 255 };
		BuilderHelpers::AddCenteredTxt(m_Scene, "NOW IT IS BEGINNING OF A", fontSoftRef, centerX, 140.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "FANTASTIC STORY!! LETS MAKE A", fontSoftRef, centerX, 200.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "JOURNEY TO THE CAVE OF MONSTERS!", fontSoftRef, centerX, 260.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "GOOD LUCK!", fontSoftRef, centerX, 320.f, color);
	}
}
