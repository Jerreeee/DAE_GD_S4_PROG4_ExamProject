#pragma once
#include "JREngine/Asset/IAssetImporter.h"
#include "Assets/AnimsData.h"

namespace BubbleBobble
{
	class AnimDataImporter : public JRE::IAssetImporter
	{
	public:
		AnimDataImporter(const std::filesystem::path& filepath);

		static JRE::AssetRef<JRE::Asset> ImportAsset(JRE::AssetHandle handle, const JRE::AssetMetadata& metadata);

		virtual JRE::AssetMetadata GetMetadata() const override;
	private:
		std::filesystem::path m_Path{};
	};
}
