#include <stdexcept>
#include <fstream>
#include <sstream>
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/Sprite.h"
#include "Level/Level.h"
#include "Level/LevelImporter.h"

namespace BubbleBobble
{
	static bool s_Registered = []()
		{
			JRE::AssetImporter::GetInstance().RegisterImporter(Level::GetStaticType(), LevelImporter::ImportAsset);
			return true;
		}();

	LevelImporter::LevelImporter(const std::filesystem::path& levelDir)
		: m_Path{ JRE::AssetImporter::GetInstance().GetFullDatapath(levelDir) }
	{
		if (!std::filesystem::exists(m_Path) || !std::filesystem::is_directory(m_Path))
			throw std::runtime_error("LevelDir does not exist or is not a directory");
	}

	JRE::AssetRef<JRE::Asset> LevelImporter::ImportAsset(JRE::AssetHandle, const JRE::AssetMetadata& metadata)
	{
		std::filesystem::path relPath = metadata.filepath.lexically_relative(JRE::AssetImporter::GetInstance().GetDatapath());

		auto levelRef = JRE::CreateAssetRef<Level>();

		//Get all the sprite names
		std::vector<std::filesystem::path> spriteNames{};
		for (const auto& entry : std::filesystem::directory_iterator(metadata.filepath))
			if (entry.is_regular_file() && entry.path().extension() == ".png")
				spriteNames.push_back(entry.path().filename());

		//Load all the sprites
		std::vector<JRE::AssetRef<JRE::Sprite>> sprites{};
		for (auto& spriteName : spriteNames)
		{
			auto filePath = std::filesystem::path(relPath / spriteName);
			auto texture = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::TextureImporter(filePath)));
			auto textureRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(texture);
			auto sprite = JRE::CreateAssetRef<JRE::Sprite>(JRE::SoftAssetRef<JRE::Texture2D>(textureRef));
			sprites.emplace_back(sprite);
		}

		//Open the level txt file for reading
		std::filesystem::path txtFilePath(metadata.filepath / "Data.txt");
		std::fstream fStream(txtFilePath);
		if (!fStream.is_open())
			throw std::runtime_error("LevelImporter::ImportAsset | Couldnt open file: " + relPath.string());

		std::vector<Level::SpritePos> spritePositions{};
		//Go through all the lines and store the sprite and pos
		std::string line{};
		while (std::getline(fStream, line))
		{
			if (line.empty() || line[0] == '/')
				continue;

			std::stringstream ss(line);
			std::string token{};
			std::vector<std::string> tokens{};

			while (std::getline(ss, token, ','))
			{
				//Trim quotes "" if present
				if (!token.empty() && token.front() == '"' && token.back() == '"')
					token = token.substr(1, token.size() - 2);
				tokens.push_back(token);
			}

			const std::string& name = tokens[0];
			auto it = std::find_if(spriteNames.begin(), spriteNames.end(),
				[&](const std::filesystem::path& path)
				{
					return name == path.stem();
				});
			if (it == spriteNames.end()) //invalid sprite name
				continue;
			size_t spriteIdx = std::distance(spriteNames.begin(), it);
			int x = std::stoi(tokens[1]);
			int y = std::stoi(tokens[2]);

			spritePositions.emplace_back(Level::SpritePos{ spriteIdx, glm::vec2{x, y} });
		}

		levelRef->SetSprites(sprites);
		levelRef->SetSpritePositions(spritePositions);
		return levelRef;
	}
	JRE::AssetMetadata LevelImporter::GetMetadata() const
	{
		return JRE::AssetMetadata{ Level::GetStaticType().data(), m_Path, "", true };
	}
}
