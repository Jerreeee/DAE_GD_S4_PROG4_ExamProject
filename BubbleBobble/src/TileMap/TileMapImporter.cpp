#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/AssetRegistry.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/Sprite.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileMapImporter.h"

namespace BubbleBobble
{
	static bool s_Registered = []()
		{
			JRE::AssetImporter::GetInstance().RegisterImporter(TileMap::GetStaticType(), TileMapImporter::ImportAsset);
			return true;
		}();

	TileMapImporter::TileMapImporter(const std::filesystem::path& path)
		: m_Path{ path }   // relative only, no GetFullDatapath, no existence check
	{
	}

	JRE::AssetRef<JRE::Asset> TileMapImporter::ImportAsset(JRE::AssetHandle, const JRE::AssetMetadata& metadata)
	{
		// metadata.filepath is relative: e.g. "Levels/1/TileMapData.txt"
		auto relLevelDir = metadata.filepath.parent_path(); // e.g. "Levels/1"

		auto levelRef = JRE::CreateAssetRef<TileMap>();

		auto fullTxtPath = JRE::AssetImporter::GetInstance().GetFullDatapath(metadata.filepath);
		std::fstream fStream(fullTxtPath);
		if (!fStream.is_open())
			throw std::runtime_error("TileMapImporter::ImportAsset | Cannot open: " + metadata.filepath.string());

		// First pass: collect all unique sprite stems (from T lines) and positions
		std::vector<std::string> spriteStems{};
		std::vector<TileMap::SpritePos> spritePositions{};
		std::vector<TileMap::ColliderInfo> collisionRects{};

		std::string line{};
		while (std::getline(fStream, line))
		{
			if (line.empty() || line[0] == '/') continue;

			std::stringstream ss(line);
			std::string type{};
			std::getline(ss, type, ' ');

			std::string token{};
			std::vector<std::string> tokens{};
			while (std::getline(ss, token, ','))
			{
				if (!token.empty() && token.front() == '"' && token.back() == '"')
					token = token.substr(1, token.size() - 2);
				tokens.push_back(token);
			}

			if (type == "T")
			{
				const std::string& stem = tokens[0];
				if (std::find(spriteStems.begin(), spriteStems.end(), stem) == spriteStems.end())
					spriteStems.push_back(stem);
				size_t idx = std::find(spriteStems.begin(), spriteStems.end(), stem) - spriteStems.begin();
				int x = std::stoi(tokens[1]);
				int y = std::stoi(tokens[2]);
				spritePositions.emplace_back(TileMap::SpritePos{ idx, glm::vec2{x, y} });
			}
			else if (type == "C")
				AddCollisionRect(tokens, collisionRects);
		}

		// Load sprites by looking up pre-registered handles from the registry
		std::vector<JRE::AssetRef<JRE::Sprite>> sprites{};
		for (const auto& stem : spriteStems)
		{
			std::string relPath = (relLevelDir / (stem + ".png")).generic_string();
			JRE::AssetHandle texHandle = JRE::AssetRegistry::GetInstance().GetHandleAtPath(relPath);
			auto texture = JRE::ResourceManager::GetAsset<JRE::Texture2D>(texHandle);
			auto sprite = JRE::CreateAssetRef<JRE::Sprite>(JRE::SoftAssetRef<JRE::Texture2D>(texture));
			sprites.emplace_back(sprite);
		}

		levelRef->SetSprites(sprites);
		levelRef->SetSpritePositions(spritePositions);
		levelRef->SetCollisionRects(collisionRects);
		return levelRef;
	}

	JRE::AssetMetadata TileMapImporter::GetMetadata() const
	{
		return JRE::AssetMetadata{ TileMap::GetStaticType().data(), m_Path, "", true, GetDeclaredDependencies() };
	}

	void TileMapImporter::AddCollisionRect(const std::vector<std::string>& tokens,
		std::vector<TileMap::ColliderInfo>& collisionRect)
	{
		bool isPlatform = (tokens[0] == "1");
		float leftTopX = static_cast<float>(std::stoi(tokens[1]));
		float leftTopY = static_cast<float>(std::stoi(tokens[2]));
		float rightBottomX = static_cast<float>(std::stoi(tokens[3]));
		float rightBottomY = static_cast<float>(std::stoi(tokens[4]));
		JRE::BoxShape boxShape{};
		boxShape.offset = glm::vec2(leftTopX, leftTopY);
		boxShape.width = rightBottomX - leftTopX;
		boxShape.height = rightBottomY - leftTopY;
		collisionRect.emplace_back(TileMap::ColliderInfo{ boxShape, isPlatform });
	}
}
