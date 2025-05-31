#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/FontImporter.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Rendering/TextRendererComponent.h"

#include "MainMenuBuilder.h"

namespace BubbleBobble
{
	void MainMenuBuilder::Build(JRE::Scene& scene)
	{
		auto logo = std::make_unique<JRE::GameObject>("Logo");
		auto spriteRenderCmp = logo->AddComponent<JRE::SpriteRendererComponent>();
		auto logoHandle = JRE::AssetImporter::GetInstance().ImportAsset(JRE::TextureImporter("HUD/Logo.png"));
		auto spriteRef = JRE::CreateAssetRef<JRE::Sprite>(JRE::SoftAssetRef<JRE::Texture2D>(logoHandle));
		spriteRenderCmp->SetSprite(spriteRef);
		//const JRE::Region& region = spriteRef->GetSrcRegion();
		//logo->SetWorldPosition();

		//load font
		auto fontImporter = JRE::FontImporter("Fonts/Pixel_NES.otf");
		fontImporter.SetSize(20);
		auto fontHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(fontImporter));
		auto fontSoftRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

		auto singlePlayerTxt = std::make_unique<JRE::GameObject>("ForSingelPlayerTxt");
		singlePlayerTxt->AddComponent<JRE::TextRendererComponent>("<- FOR SINGLEPLAYER", fontSoftRef);

		auto multiplayerTxt = std::make_unique<JRE::GameObject>("ForSingelPlayerTxt");
		multiplayerTxt->AddComponent<JRE::TextRendererComponent>("^ FOR MULTIPLAYER", fontSoftRef);

		auto versusTxt = std::make_unique<JRE::GameObject>("ForSingelPlayerTxt");
		versusTxt->AddComponent<JRE::TextRendererComponent>("-> FOR VERSUS", fontSoftRef);

		scene.Add(std::move(singlePlayerTxt));
		scene.Add(std::move(multiplayerTxt));
		scene.Add(std::move(versusTxt));
		scene.Add(std::move(logo));
	}
}
