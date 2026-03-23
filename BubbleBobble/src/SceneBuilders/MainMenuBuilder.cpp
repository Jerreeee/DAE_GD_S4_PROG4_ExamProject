#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Rendering/TextRendererComponent.h"

#include "Assets/AnimsData.h"
#include "Utils.h"
#include "SceneBuilders/BuilderHelpers.h"
#include "MainMenuBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	MainMenuBuilder::MainMenuBuilder(JRE::Scene& scene)
		: m_Scene{ scene }
	{
	}
	void MainMenuBuilder::Build()
	{
		AssetHandle animsDataHandle = GetRegisteredHandle("Anims/Logo.txt");
		AssetRef<AnimsData> animsDataRef = ResourceManager::GetAsset<AnimsData>(animsDataHandle);
		const AnimData& animData = animsDataRef->dataVec[0];
		auto logo = std::make_unique<JRE::GameObject>("Logo");
		logo->AddComponent<JRE::SpriteRendererComponent>();
		auto spriteAnimCmp = logo->AddComponent<JRE::SpriteAnimatorComponent>();
		auto clipRef = Utils::CreateAnimationClipFromAnimData(animData);
		spriteAnimCmp->AddClip(animData.animName, clipRef, true);
		logo->SetWorldPosition(117.f, 50.f);

		auto fontHandle = GetRegisteredHandle("Fonts/Pixel_NES.otf/@20");
		auto fontSoftRef = JRE::SoftAssetRef<JRE::Font>(fontHandle);

		const int centerX = 384;
		SDL_Color color = { 255, 255, 255, 255 };
		BuilderHelpers::AddCenteredTxt(m_Scene, "<- FOR SINGLEPLAYER", fontSoftRef, centerX, 520.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "^ FOR MULTIPLAYER", fontSoftRef, centerX, 560.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "-> FOR VERSUS", fontSoftRef, centerX, 600.f, color);

		m_Scene.Add(std::move(logo));
	}
}
