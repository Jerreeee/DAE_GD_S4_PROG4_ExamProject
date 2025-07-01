#include <fstream>
#include <sstream>
#include <stdexcept>
#include "JREngine/Asset/AssetImporter.h"
#include "Assets/LevelDataImporter.h"

using namespace JRE;

namespace BubbleBobble
{
	static bool s_Registered = []()
		{
			AssetImporterRegistry::GetInstance().RegisterImporter(LevelData::GetStaticTypeName(), LevelDataImporter::ImportAsset);
			return true;
		}();

	LevelDataImporter::LevelDataImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporterRegistry::GetInstance().GetFullDatapath(filepath) }
	{
	}

	AssetRef<Asset> LevelDataImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
		std::ifstream fStream(metadata.filepath.string().c_str());
		if (!fStream)
			throw std::runtime_error("Failed to open level file: " + metadata.filepath.string());

		auto levelDataRef = CreateAssetRef<LevelData>();

		std::string line{};
		std::getline(fStream, line); // Skip header
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

			if (type == "P")
				AddPlayer(tokens, levelDataRef->players);
			else if (type == "Z" || type == "M")
				AddEnemy(tokens, type, levelDataRef->enemies);
		}

		return levelDataRef;
	}

	AssetMetadata LevelDataImporter::GetMetadata() const
	{
		return AssetMetadata{ LevelData::GetStaticTypeName().data(), m_Path, "", false };
	}
	void LevelDataImporter::AddPlayer(const std::vector<std::string>& tokens,
		std::vector<glm::vec2>& players)
	{
		assert(tokens.size() == 3 && "Ill-formatted player line");
		glm::vec2 pos{ std::stof(tokens[0]), std::stof(tokens[1]) };
		players.emplace_back(pos);
	}
	void LevelDataImporter::AddEnemy(const std::vector<std::string>& tokens, const std::string& type,
		std::vector<EnemyInfo>& enemies)
	{
		assert(tokens.size() == 2 && "Ill-formatted player line");
		EnemyInfo enemy{};
		enemy.type = type == "Z" ? EnemyType::Zenchan : EnemyType::Maita;
		enemy.pos = { std::stof(tokens[0]), std::stof(tokens[1]) };
		enemies.emplace_back(enemy);
	}
}
