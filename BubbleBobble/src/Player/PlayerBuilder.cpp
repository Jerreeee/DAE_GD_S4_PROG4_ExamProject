#include <fstream>
#include <sstream>

#include "JREngine/Asset/Asset.h"
#include "JREngine/Input/InputManager.h"
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/SpriteEditor.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Animation/SpriteAnimationClip.h"
#include "JREngine/Rendering/SpriteRendererComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "Assets/AnimsDataImporter.h"
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
        player->AddComponent<PlayerScriptComponent>();
        player->AddComponent<PlayerControllerComponent>(static_cast<size_t>(m_ActionMapIdx));
		SetAnimations(pSpriteAnimatorCmp);
	}
	void PlayerBuilder::SetAnimations(JRE::SpriteAnimatorComponent* pCmp)
	{
        auto animsDataImporter = AnimDataImporter(m_AnimPath);
        AssetHandle animsDataHandle = AssetImporter::GetInstance().ImportAsset(std::move(animsDataImporter));
        AssetRef<AnimsData> animsDataRef = ResourceManager::GetAsset<AnimsData>(animsDataHandle);

        for (const AnimData& animData : animsDataRef->dataVec)
        {
            auto texture = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::TextureImporter(animData.path)));
            auto textureRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(texture);
            auto sprites = JRE::SpriteEditor::SplitTexture2D(textureRef, animData.frameCount, animData.cols, animData.rows);

            std::vector<JRE::SoftAssetRef<JRE::Sprite>> softSprites;
            softSprites.reserve(sprites.size());
            for (const auto& sprite : sprites)
                softSprites.emplace_back(sprite);

            auto clipRef = JRE::CreateAssetRef<JRE::SpriteAnimationClip>(softSprites, animData.fps);
            pCmp->AddClip(animData.animName, clipRef);
        }
	}
}
