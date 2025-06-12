#include "JREngine/Scene/GameObject.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

#include "Utils.h"
#include "CollisionLayers.h"
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
        auto box2DColliderCmp = pZenchan->AddComponent<JRE::Box2DColliderComponent>();
        box2DColliderCmp->m_Shape.offset = glm::vec2(0.f, 0.f);
        box2DColliderCmp->m_Shape.width = 48.f;
        box2DColliderCmp->m_Shape.height = 48.f;
        box2DColliderCmp->m_Properties.layer = CollisionLayer::Enemy;
        box2DColliderCmp->m_Properties.mask = CollisionMask::Enemy;
        pZenchan->AddComponent<ZenchanScriptComponent>();

        //Zenchan is controlled by AI and has the player as its target
        if (m_pTarget)
        {
            auto pAIControllerCmp = pZenchan->AddComponent<ZenchanAIController>();
            pAIControllerCmp->SetTarget(*m_pTarget);
        }
        pSpriteAnimatorCmp->SetActiveClip("Run");
	}
}
