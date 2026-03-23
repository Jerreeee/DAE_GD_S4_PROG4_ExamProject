#pragma once
#include "JREngine/Asset/IAssetImporter.h"
#include "JREngine/Asset/Font.h"

namespace JRE
{
	class FontImporter : public IAssetImporter
	{
	public:
		FontImporter(const std::filesystem::path& filepath);

		static AssetRef<Asset> Load(AssetHandle handle, const AssetMetadata& metadata);

		virtual AssetMetadata GetMetadata() const override;

		std::string GetUniqueID() const;

		FontImporter& SetSize(int size);
	private:
		std::filesystem::path m_Path{};
		int m_Size{ 16 };
	};
}
