#include <vector>
#include "SDL.h"
#undef main
#include "glm.hpp"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/FontImporter.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Scene/GameObject.h"

#include "Assets/AnimsDataImporter.h"
#include "Utils.h"
#include "SceneBuilders/BuilderHelpers.h"
#include "LoadingScreenBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	LoadingMenuBuilder::LoadingMenuBuilder(JRE::Scene& scene)
		: m_Scene{ scene }
	{
	}
	void LoadingMenuBuilder::Build()
	{
		//load font
		auto fontImporter = FontImporter("Fonts/Pixel_NES.otf");
		fontImporter.SetSize(20);
		auto fontHandle = AssetImporter::GetInstance().ImportAsset(std::move(fontImporter));
		auto fontSoftRef = SoftAssetRef<Font>(fontHandle);

		const int centerX = 384;
		SDL_Color color = { 255, 0, 0, 255 };
		BuilderHelpers::AddCenteredTxt(m_Scene, "NOW IT IS BEGINNING OF A", fontSoftRef, centerX, 140.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "FANTASTIC STORY!! LETS MAKE A", fontSoftRef, centerX, 200.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "JOURNEY TO THE CAVE OF MONSTERS!", fontSoftRef, centerX, 260.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "GOOD LUCK!", fontSoftRef, centerX, 320.f, color);

		//anims
		std::vector<std::string> animsDataNames{ "Anims/Intro_P1.txt", "Anims/Intro_P2.txt" };
		std::vector<glm::vec2> positions{
			glm::vec2{238.f, 450.f},
			glm::vec2{434.f, 450.f}
		};
		for (int i{}; i < animsDataNames.size(); ++i)
		{
			auto animsDataImporter = AnimDataImporter(animsDataNames[i]);
			AssetHandle animsDataHandle = AssetImporter::GetInstance().ImportAsset(std::move(animsDataImporter));
			AssetRef<AnimsData> animsDataRef = ResourceManager::GetAsset<AnimsData>(animsDataHandle);
			const AnimData& animData = animsDataRef->dataVec[0];
			auto go = std::make_unique<JRE::GameObject>(animData.animName + "GO");
			go->AddComponent<SpriteRendererComponent>();
			auto spriteAnimCmp = go->AddComponent<JRE::SpriteAnimatorComponent>();
			auto clipRef = Utils::CreateAnimationClipFromAnimData(animData);
			spriteAnimCmp->SetSpriteAnimationClip(clipRef);
			go->SetWorldPosition(positions[i].x, positions[i].y);
			m_Scene.Add(std::move(go));
		}
	}
}
