#pragma once
#include "JREngine/Asset/IAssetImporter.h"

namespace JRE
{
	class TextureImporter : public IAssetImporter
	{
	public:
		TextureImporter(const std::filesystem::path& filepath);

		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);

		virtual AssetMetadata GetMetadata() const override;
	private:
		std::filesystem::path m_Path{};
	};
}
