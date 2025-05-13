#include <fstream>
#include <sstream>
#include <iostream>
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/Sprite.h"
#include "Level/Level.h"

namespace BubbleBobble
{
	Level::Level(const std::filesystem::path& path)
	{
		LoadSprites(path);
		LoadTileData(path);
	}

	const std::vector<Level::DrawInfo>& Level::GetDrawInfo() const
	{
		return m_DrawInfo;
	}

	void Level::LoadSprites(const std::filesystem::path& path)
	{
		std::vector<std::string> spriteNames{};

		if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path))
		{
			std::cerr << "Invalid directory: " << path << '\n';
			return;
		}

		for (const auto& entry : std::filesystem::directory_iterator(path))
			if (entry.is_regular_file() && entry.path().extension() == ".png")
				spriteNames.push_back(entry.path().filename().string());

		for (auto& spriteName : spriteNames)
		{
			auto filePath = std::filesystem::path(path / spriteName);
			auto texture = JRE::AssetImporter::GetInstance().ImportAsset(std::move(JRE::TextureImporter(filePath)));
			auto textureRef = JRE::ResourceManager::GetAsset<JRE::Texture2D>(texture);
			auto sprite = JRE::CreateAssetRef<JRE::Sprite>(JRE::SoftAssetRef<JRE::Texture2D>(textureRef));
			m_Sprites.emplace_back(sprite);
			m_SpriteNames.emplace_back(spriteName);
		}
	}

	void Level::LoadTileData(const std::filesystem::path& path)
	{
		std::fstream fStream(path);
		if (!fStream.is_open())
			throw std::runtime_error("Level::Level | Couldnt open file: " + path.string());

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
			int x = std::stoi(tokens[1]);
			int y = std::stoi(tokens[2]);

			auto it = std::find(m_SpriteNames.begin(), m_SpriteNames.end(), name);
			if (it == m_SpriteNames.end()) //invalid sprite name
			{
				std::cout << "Level::LoadTileData | Path: " << path << " | Invalid name: " << name << "\n";
				continue;
			}
			size_t spriteIdx = std::distance(m_SpriteNames.begin(), it);
			m_DrawInfo.emplace_back(DrawInfo{ spriteIdx, glm::vec2{x, y} });
		}
	}
}
