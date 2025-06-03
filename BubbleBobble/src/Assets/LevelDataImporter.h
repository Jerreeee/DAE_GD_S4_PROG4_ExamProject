#pragma once
#include "JREngine/Asset/IAssetImporter.h"
#include "Assets/LevelData.h"

namespace BubbleBobble
{
	class LevelDataImporter : public JRE::IAssetImporter
	{
	public:
		LevelDataImporter(const std::filesystem::path& filepath);

		static JRE::AssetRef<JRE::Asset> ImportAsset(JRE::AssetHandle handle, const JRE::AssetMetadata& metadata);

		virtual JRE::AssetMetadata GetMetadata() const override;
	private:
		static void AddPlayer(const std::vector<std::string>& tokens,
			std::vector<glm::vec2>& players);
		static void AddEnemy(const std::vector<std::string>& tokens, const std::string& type,
			std::vector<EnemyInfo>& enemies);

		std::filesystem::path m_Path{};
	};
}
