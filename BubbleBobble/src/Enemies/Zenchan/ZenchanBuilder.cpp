#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "Utils.h"
#include "Enemies/Zenchan/ZenchanScriptComponent.h"
#include "Enemies/Zenchan/ZenchanAIController.h"
#include "Enemies/Zenchan/ZenchanBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
    ZenchanBuilder& ZenchanBuilder::SetupAIController(const JRE::GameObject* pTarget)
    {
        m_pTarget = pTarget;
        return *this;
    }
    void ZenchanBuilder::Build(std::unique_ptr<JRE::GameObject>& pZenchan)
	{
        pZenchan->AddComponent<JRE::SpriteRendererComponent>();
        auto pSpriteAnimatorCmp = pZenchan->AddComponent<JRE::SpriteAnimatorComponent>();
        Utils::AddAnimsToSpriteAnimatorComponent("Anims/ZenChan.txt", *pSpriteAnimatorCmp);
        pZenchan->AddComponent<ZenchanScriptComponent>();

        //Zenchan is controlled by AI and has the player as its target
        if (m_pTarget)
        {
            auto pAIControllerCmp = pZenchan->AddComponent<ZenchanAIController>();
            pAIControllerCmp->m_pTarget = m_pTarget;
        }
        pSpriteAnimatorCmp->SetActiveClip("Run");
	}
}
