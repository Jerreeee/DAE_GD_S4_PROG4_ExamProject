#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

#include "Utils.h"
#include "EngineSetup.h"
#include "Bubble/BubbleScript.h"
#include "Bubble/BubbleBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
    std::unique_ptr<JRE::GameObject> BubbleBuilder::Build(const std::string& playerName, const glm::vec2& position, int direction)
    {
        auto bubble = std::make_unique<GameObject>("Bubble");
        bubble->m_PersistenceScope = PersistenceLayer::SceneTemporary;
        bubble->SetWorldPosition(position.x, position.y);

        bubble->AddComponent<SpriteRendererComponent>();
        auto spriteAnimCmp = bubble->AddComponent<SpriteAnimatorComponent>();
        Utils::AddAnimsToSpriteAnimatorComponent("Anims/Bubble.txt", *spriteAnimCmp);
        spriteAnimCmp->SetActiveClip(playerName + "Bubble");
        auto box2DColliderCmp = bubble->AddComponent<Box2DColliderComponent>();
        box2DColliderCmp->m_Shape.width = 48.f;
        box2DColliderCmp->m_Shape.height = 48.f;
        box2DColliderCmp->m_Properties.layer = CollisionLayer::Bubble;
        box2DColliderCmp->m_Properties.mask = CollisionMask::Bubble;
        bubble->AddComponent<BubbleScript>(direction);
        return bubble;
    }
}
