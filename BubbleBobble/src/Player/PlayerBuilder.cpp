#include <fstream>
#include <sstream>

#include "JREngine/Scene/GameObject.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "Utils.h"
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
        player->AddComponent<PlayerScriptComponent>();
        player->AddComponent<PlayerControllerComponent>(static_cast<size_t>(m_ActionMapIdx));
	}
}
