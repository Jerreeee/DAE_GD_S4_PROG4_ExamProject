#include <stdexcept>
#include <fstream>
#include <sstream>
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/Sprite.h"
#include "Level/TileMap.h"
#include "Level/TileMapImporter.h"

namespace BubbleBobble
{
	static bool s_Registered = []()
		{
			JRE::AssetImporter::GetInstance().RegisterImporter(TileMap::GetStaticType(), TileMapImporter::ImportAsset);
			return true;
		}();

	TileMapImporter::TileMapImporter(const std::filesystem::path& path)
		: m_Path{ JRE::AssetImporter::GetInstance().GetFullDatapath(path) }
	{
		if (!std::filesystem::exists(m_Path))
			throw std::runtime_error("TileMapData.txt does not exist");
	}

	JRE::AssetRef<JRE::Asset> TileMapImporter::ImportAsset(JRE::AssetHandle, const JRE::AssetMetadata& metadata)
	{
		auto levelDir = metadata.filepath.parent_path();
		auto relLevelDir = std::filesystem::relative(levelDir, JRE::AssetImporter::GetInstance().GetDatapath());
		//std::filesystem::path relPath = metadata.filepath.lexically_relative(JRE::AssetImporter::GetInstance().GetDatapath());

		auto levelRef = JRE::CreateAssetRef<TileMap>();

		//Get all the sprite names
		std::vector<std::filesystem::path> spriteNames{};
		for (const auto& entry : std::filesystem::directory_iterator(levelDir))
			if (entry.is_regular_file() && entry.path().extension() == ".png")
				spriteNames.push_back(entry.path().filename());

		//Load all the sprites
		std::vector<JRE::AssetRef<JRE::Sprite>> sprites{};
		for (auto& spriteName : spriteNames)
		{
			auto filePath = std::filesystem::path(relLevelDir / spriteName);
			auto texture = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::TextureImporter(filePath)));
			auto textureRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(texture);
			auto sprite = JRE::CreateAssetRef<JRE::Sprite>(JRE::SoftAssetRef<JRE::Texture2D>(textureRef));
			sprites.emplace_back(sprite);
		}

		//Open the level txt file for reading
		std::filesystem::path txtFilePath(metadata.filepath);
		std::fstream fStream(txtFilePath);
		if (!fStream.is_open())
			throw std::runtime_error("LevelImporter::ImportAsset | Couldnt open file: " + metadata.filepath.string());

		std::vector<TileMap::SpritePos> spritePositions{};
		std::vector<JRE::Region> collisionRects{};

		std::string line{};
		while (std::getline(fStream, line))
		{
			if (line.empty() || line[0] == '/')
				continue;

			std::stringstream ss(line);
			std::string type{};
			std::getline(ss, type, ' '); //extract part up to first space to check the type of information on that line

			std::string token{};
			std::vector<std::string> tokens{};
			while (std::getline(ss, token, ','))
			{
				if (!token.empty() && token.front() == '"' && token.back() == '"')
					token = token.substr(1, token.size() - 2);
				tokens.push_back(token);
			}

			if (type == "T")
				AddSpritePosition(tokens, spriteNames, spritePositions);
			else if (type == "C")
				AddCollisionRect(tokens, collisionRects);
		}

		levelRef->SetSprites(sprites);
		levelRef->SetSpritePositions(spritePositions);
		levelRef->SetCollisionRects(collisionRects);
		return levelRef;
	}
	JRE::AssetMetadata TileMapImporter::GetMetadata() const
	{
		return JRE::AssetMetadata{ TileMap::GetStaticType().data(), m_Path, "", true };
	}
	void TileMapImporter::AddSpritePosition(const std::vector<std::string>& tokens,
		std::vector<std::filesystem::path>& spriteNames,
		std::vector<TileMap::SpritePos>& spritePositions)
	{
		const std::string& name = tokens[0];
		auto it = std::find_if(spriteNames.begin(), spriteNames.end(),
			[&](const std::filesystem::path& path)
			{
				return name == path.stem();
			});
		if (it == spriteNames.end()) //invalid sprite name
			throw std::runtime_error("Invalid sprite name");
		size_t spriteIdx = std::distance(spriteNames.begin(), it);
		int x = std::stoi(tokens[1]);
		int y = std::stoi(tokens[2]);

		spritePositions.emplace_back(TileMap::SpritePos{ spriteIdx, glm::vec2{x, y} });
	}
	void TileMapImporter::AddCollisionRect(const std::vector<std::string>& tokens,
		std::vector<JRE::Region>& collisionRect)
	{
		int leftTopX = std::stoi(tokens[0]);
		int leftTopY = std::stoi(tokens[1]);
		int rightBottomX = std::stoi(tokens[2]);
		int rightBottomY = std::stoi(tokens[3]);
		collisionRect.emplace_back(JRE::Region{ leftTopX, leftTopY, rightBottomX - leftTopX, rightBottomY - leftTopY });
	}
}
