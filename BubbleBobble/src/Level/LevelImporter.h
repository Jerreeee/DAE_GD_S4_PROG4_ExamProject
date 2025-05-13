#pragma once
#include "JREngine/Asset/IAssetImporter.h"

namespace BubbleBobble
{
	class LevelImporter final : public JRE::IAssetImporter
	{
	public:
		LevelImporter(const std::filesystem::path& filepath);

		static JRE::AssetRef<JRE::Asset> ImportAsset(JRE::AssetHandle handle, const JRE::AssetMetadata& metadata);

		virtual JRE::AssetMetadata GetMetadata() const override;
	private:
		std::filesystem::path m_Path{};
	};
}
