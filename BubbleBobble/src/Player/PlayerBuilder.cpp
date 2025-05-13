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
        auto pScriptCmp = player->AddComponent<ScriptComponent>();
        assert(pScriptCmp && "pScriptCmp wass nullptr");
		SetAnimations(pScriptCmp);
	}
	void Builder::SetAnimations(ScriptComponent* pScriptCmp)
	{
		std::ifstream fStream(m_AnimPath.string().c_str());
		if (!fStream)
			throw std::runtime_error("Failed to open animation file: " + m_AnimPath.string());

		std::string line;
		std::getline(fStream, line);
        while (std::getline(fStream, line))
        {
            if (line.empty() || line[0] == '/')
                continue;

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(ss, token, ','))
            {
                //Trim quotes "" if present
                if (!token.empty() && token.front() == '"' && token.back() == '"')
                    token = token.substr(1, token.size() - 2);
                tokens.push_back(token);
            }

            if (tokens.size() != 7)
                throw std::runtime_error("Incorrectly formed animation data: " + line);

            const std::string& path = tokens[0];
            int frameCount = std::stoi(tokens[1]);
            int cols = std::stoi(tokens[2]);
            int rows = std::stoi(tokens[3]);
            int fps = std::stoi(tokens[4]);
            //bool isPong = (tokens[5] == "true");
            const std::string& animName = tokens[6];

            // Now create the animation using the extracted data
            auto texture = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::TextureImporter(path)));
            auto textureRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(texture);
            auto sprites = JRE::SpriteEditor::SplitTexture2D(textureRef, frameCount, cols, rows);

            std::vector<JRE::SoftAssetRef<JRE::Sprite>> softSprites;
            softSprites.reserve(sprites.size());
            for (const auto& sprite : sprites)
                softSprites.emplace_back(sprite);

            auto animRef = JRE::CreateAssetRef<JRE::SpriteAnimationClip>(softSprites, fps);
            pScriptCmp->SetAnimation(animName, animRef);
        }
	}
}
