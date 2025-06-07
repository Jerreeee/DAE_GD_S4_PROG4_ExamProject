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
#include "JREngine/Physics/RigidBody2DComponent.h"
#include "JREngine/Animation/SpriteAnimatorComponent.h"

#include "Assets/AnimsDataImporter.h"
#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerBuilder.h"

using namespace JRE;
using namespace JRE::Input;

namespace BubbleBobble::Player
{
	Builder& Builder::SetAnimationPath(const std::filesystem::path& path)
	{
		m_AnimPath = path;
        return *this;
	}
    Builder& Builder::SetActionMap(const ActionMap& actionMap)
    {
        m_pActionMap = &actionMap;
        return *this;
    }
	void Builder::Build(std::unique_ptr<JRE::GameObject>& player)
	{
        player->AddComponent<JRE::SpriteRendererComponent>();
        //player->AddComponent<JRE::RigidBody2DComponent>();
        player->AddComponent<JRE::SpriteAnimatorComponent>();
        auto pScriptCmp = player->AddComponent<Player::ScriptComponent>(*m_pActionMap);
        assert(pScriptCmp && "pScriptCmp wass nullptr");
		SetAnimations(pScriptCmp);
        player->m_Persistant = true;
	}
	void Builder::SetAnimations(ScriptComponent* pScriptCmp)
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

            auto animRef = JRE::CreateAssetRef<JRE::SpriteAnimationClip>(softSprites, animData.fps);
            pScriptCmp->SetAnimation(animData.animName, animRef);
        }
	}
}
