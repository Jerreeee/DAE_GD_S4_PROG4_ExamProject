#pragma once
#include "JREngine/Asset/IAssetImporter.h"

namespace JRE
{
	class SoundClipImporter : public IAssetImporter
	{
	public:
		SoundClipImporter(const std::filesystem::path& filepath);

		static AssetRef<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);

		virtual AssetMetadata GetMetadata() const override;
	private:
		std::filesystem::path m_Path{};
	};
}
