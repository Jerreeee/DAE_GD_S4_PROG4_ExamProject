#include <fstream>
#include <sstream>

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

#include "Player/PlayerScriptComponent.h"
#include "Player/PlayerBuilder.h"
#include "FileIO.h"

namespace BubbleBobble::Player
{
	Builder& Builder::SetAnimationPath(const std::filesystem::path& path)
	{
		m_AnimPath = path;
        return *this;
	}
	void Builder::Build(std::unique_ptr<JRE::GameObject>& player)
	{
        player->SetLocalPosition(216, 180);
        player->AddComponent<JRE::SpriteRendererComponent>();
        player->AddComponent<JRE::RigidBody2DComponent>();
        player->AddComponent<JRE::SpriteAnimatorComponent>();
        auto pScriptCmp = player->AddComponent<Player::ScriptComponent>();
        assert(pScriptCmp && "pScriptCmp wass nullptr");
		SetAnimations(pScriptCmp);
	}
	void Builder::SetAnimations(ScriptComponent* pScriptCmp)
	{
        std::vector<AnimData> animDataVec = FileIO::GetAnimData(m_AnimPath);

        for (const AnimData& animData : animDataVec)
        {
            // Now create the animation using the extracted data
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
