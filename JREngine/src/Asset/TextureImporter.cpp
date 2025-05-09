#include "Asset/Texture2D.h"
#include "Asset/AssetImporter.h"
#include "Asset/TextureImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::GetInstance().RegisterImporter(AssetType::Texture2D, TextureImporter::ImportAsset);
			return true;
		}();

	TextureImporter::TextureImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporter::GetInstance().GetFullDatapath(filepath) }
	{
	}

	Ref<Asset> TextureImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
		return CreateRef<Texture2D>(metadata.filepath);
	}

	AssetMetadata TextureImporter::GetMetadata() const
	{
		return AssetMetadata{ AssetType::Texture2D, m_Path, "", false };
	}
}
