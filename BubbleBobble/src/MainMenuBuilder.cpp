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

#include "MainMenuBuilder.h"

namespace BubbleBobble
{
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

		AddTextAtPos("<- FOR SINGLEPLAYER", fontSoftRef, 520.f);
		AddTextAtPos("^ FOR MULTIPLAYER", fontSoftRef, 560.f);
		AddTextAtPos("-> FOR VERSUS", fontSoftRef, 600.f);

		m_Scene.Add(std::move(logo));
	}
	void MainMenuBuilder::AddTextAtPos(const std::string& text, JRE::SoftAssetRef<JRE::Font> font, float y)
	{
		const int centerX = 384;

		auto go = std::make_unique<JRE::GameObject>(text);
		auto cmp = go->AddComponent<JRE::TextRendererComponent>(text, font);
		const JRE::Region& region = cmp->GetSprite().GetSrcRegion();
		int x = centerX - region.width / 2;
		go->SetWorldPosition(static_cast<float>(x), y);
		m_Scene.Add(std::move(go));
	}
}
