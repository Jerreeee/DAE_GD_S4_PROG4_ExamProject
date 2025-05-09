#pragma once
#include "JREngine/Asset/IAssetImporter.h"

namespace JRE
{
	class FontImporter : public IAssetImporter
	{
	public:
		FontImporter(const std::filesystem::path& filepath);

		static AssetRef<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);

		virtual AssetMetadata GetMetadata() const override;

		std::string GetUniqueID() const;

		FontImporter& SetSize(int size);
	private:
		std::filesystem::path m_Path{};
		int m_Size{ 16 };
	};
}
