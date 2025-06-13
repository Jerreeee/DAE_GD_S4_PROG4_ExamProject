#include <fstream>
#include <sstream>

#include "JREngine/Scene/GameObject.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"
#include "JREngine/Physics/Box2DColliderComponent.h"

#include "Utils.h"
#include "CollisionLayers.h"
#include "Components/HealthComponent.h"
#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerController.h"
#include "Player/PlayerBuilder.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble
{
	PlayerBuilder& PlayerBuilder::SetAnimationPath(const std::filesystem::path& path)
	{
		m_AnimPath = path;
        return *this;
	}
    PlayerBuilder& PlayerBuilder::SetActionMapIdx(size_t actionMapIdx)
    {
        m_ActionMapIdx = static_cast<int>(actionMapIdx);
        return *this;
    }
	void PlayerBuilder::Build(std::unique_ptr<JRE::GameObject>& player)
	{
        assert(m_ActionMapIdx != -1 && "Invalid actionMapIdx");

        player->m_Persistant = true;
        player->AddComponent<JRE::SpriteRendererComponent>();
        auto pSpriteAnimatorCmp = player->AddComponent<JRE::SpriteAnimatorComponent>();
        Utils::AddAnimsToSpriteAnimatorComponent(m_AnimPath, *pSpriteAnimatorCmp);
        auto box2DColliderCmp = player->AddComponent<JRE::Box2DColliderComponent>();
        box2DColliderCmp->m_Shape.offset = glm::vec2(4.f, 2.f);
        box2DColliderCmp->m_Shape.width = 40.f;
        box2DColliderCmp->m_Shape.height = 46.f;
        box2DColliderCmp->m_Properties.layer = CollisionLayer::Friendly;
        box2DColliderCmp->m_Properties.mask = CollisionMask::Friendly;
        auto healthCmp = player->AddComponent<HealthComponent>();
        healthCmp->SetMaxHealth(1);
        healthCmp->SetHealth(4);
        player->AddComponent<PlayerScriptComponent>();
        player->AddComponent<PlayerControllerComponent>(static_cast<size_t>(m_ActionMapIdx));
	}
}
