#include "Core/ServiceLocator.h"
#include "Asset/AssetImporter.h"
#include "Asset/TextureImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporterRegistry::GetInstance().RegisterImporter(ServiceLocator::GetAssetTypeRegistry().GetTypeInfo(Texture2D::GetStaticTypeID()).name, TextureImporter::ImportAsset);
			return true;
		}();

	TextureImporter::TextureImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporterRegistry::GetInstance().GetFullDatapath(filepath) }
	{
	}

	AssetRef<Asset> TextureImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
		return CreateAssetRef<Texture2D>(metadata.filepath);
	}

	AssetMetadata TextureImporter::GetMetadata() const
	{
		return AssetMetadata{ Texture2D::GetStaticTypeName().data(), m_Path, "", false};
	}
}
