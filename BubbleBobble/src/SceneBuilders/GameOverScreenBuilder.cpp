#include "glm.hpp"
#include <SDL.h>
#undef main
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Scene/GameObject.h"

#include "Assets/AnimsData.h"
#include "Utils.h"
#include "SceneBuilders/BuilderHelpers.h"
#include "SceneBuilders/GameOverScreenBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	GameOverScreenBuilder::GameOverScreenBuilder(JRE::Scene& scene)
		: m_Scene{ scene }
	{
	}

	void GameOverScreenBuilder::Build()
	{
		auto fontHandle = GetRegisteredHandle("Fonts/Pixel_NES.otf/@20");
		auto fontSoftRef = SoftAssetRef<Font>(fontHandle);

		const int centerX = 384;
		SDL_Color color = { 255, 0, 0, 255 };
		BuilderHelpers::AddCenteredTxt(m_Scene, "YOU FAIL...", fontSoftRef, centerX, 520.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "<- TO RESTART", fontSoftRef, centerX, 560.f, color);
		BuilderHelpers::AddCenteredTxt(m_Scene, "-> TO QUIT", fontSoftRef, centerX, 600.f, color);

		std::vector<std::string> animsDataNames{ "Anims/Fail_P1.txt", "Anims/Fail_P2.txt" };
		std::vector<glm::vec2> positions{
			glm::vec2{238.f, 450.f},
			glm::vec2{434.f, 450.f}
		};
		for (int i{}; i < static_cast<int>(animsDataNames.size()); ++i)
		{
			AssetHandle animsDataHandle = GetRegisteredHandle(animsDataNames[i]);
			AssetRef<AnimsData> animsDataRef = ResourceManager::GetAsset<AnimsData>(animsDataHandle);
			const AnimData& animData = animsDataRef->dataVec[0];
			auto go = std::make_unique<JRE::GameObject>(animData.animName + "GO");
			go->AddComponent<SpriteRendererComponent>();
			auto spriteAnimCmp = go->AddComponent<JRE::SpriteAnimatorComponent>();
			auto clipRef = Utils::CreateAnimationClipFromAnimData(animData);
			spriteAnimCmp->AddClip(animData.animName, clipRef, true);
			go->SetWorldPosition(positions[i].x, positions[i].y);
			m_Scene.Add(std::move(go));
		}
	}
}
