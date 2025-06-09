#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "Utils.h"
#include "Enemies/Zenchan/ZenchanBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	void ZenchanBuilder::Build(std::unique_ptr<JRE::GameObject>& pZenchan)
	{
        pZenchan->m_Persistant = true;
        pZenchan->AddComponent<JRE::SpriteRendererComponent>();
        auto pSpriteAnimatorCmp = pZenchan->AddComponent<JRE::SpriteAnimatorComponent>();
        Utils::AddAnimsToSpriteAnimatorComponent("Anims/ZenChan.txt", *pSpriteAnimatorCmp);
        pSpriteAnimatorCmp->SetActiveClip("Run");
	}
}
