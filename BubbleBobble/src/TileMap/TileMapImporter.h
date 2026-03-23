#pragma once
#include <glm.hpp>
#include "JREngine/Asset/IAssetImporter.h"
#include "TileMap/TileMap.h"

namespace BubbleBobble
{
	class TileMapImporter final : public JRE::IAssetImporter
	{
	public:
		TileMapImporter(const std::filesystem::path& path);

		static JRE::AssetRef<JRE::Asset> ImportAsset(JRE::AssetHandle handle, const JRE::AssetMetadata& metadata);

		virtual JRE::AssetMetadata GetMetadata() const override;
	private:
		static void AddCollisionRect(const std::vector<std::string>& tokens,
			std::vector<TileMap::ColliderInfo>& collisionRect);

		std::filesystem::path m_Path{};
	};
}
