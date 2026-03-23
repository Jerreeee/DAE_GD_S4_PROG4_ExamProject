#include "Asset/AssetImporter.h"
#include "Asset/TextureImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::GetInstance().RegisterImporter(Texture2D::GetStaticType(), TextureImporter::ImportAsset);
			return true;
		}();

	TextureImporter::TextureImporter(const std::filesystem::path& filepath) :
		m_Path{ filepath }
	{
	}

	AssetRef<Asset> TextureImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
		auto fullPath = AssetImporter::GetInstance().GetFullDatapath(metadata.filepath);
		return CreateAssetRef<Texture2D>(fullPath);
	}

	AssetMetadata TextureImporter::GetMetadata() const
	{
		return AssetMetadata{ Texture2D::GetStaticType().data(), m_Path, "", false, GetDeclaredDependencies() };
	}
}
