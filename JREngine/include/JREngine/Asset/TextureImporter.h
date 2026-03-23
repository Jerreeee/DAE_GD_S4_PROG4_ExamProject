#pragma once
#include "JREngine/Asset/IAssetImporter.h"
#include "JREngine/Asset/Texture2D.h"

namespace JRE
{
	class TextureImporter : public IAssetImporter
	{
	public:
		TextureImporter(const std::filesystem::path& filepath);

		static AssetRef<Asset> Load(AssetHandle handle, const AssetMetadata& metadata);

		virtual AssetMetadata GetMetadata() const override;
	private:
		std::filesystem::path m_Path{};
	};
}
