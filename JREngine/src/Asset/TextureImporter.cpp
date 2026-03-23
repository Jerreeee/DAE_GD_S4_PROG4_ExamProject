#include "Asset/AssetLoaderRegistry.h"
#include "Asset/AssetDatabase.h"
#include "Asset/TextureImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetLoaderRegistry::GetInstance().RegisterLoader(Texture2D::GetStaticType(), TextureImporter::Load);
			return true;
		}();

	TextureImporter::TextureImporter(const std::filesystem::path& filepath) :
		m_Path{ filepath }
	{
	}

	AssetRef<Asset> TextureImporter::Load(AssetHandle, const AssetMetadata& metadata)
	{
		auto fullPath = AssetDatabase::GetInstance().GetFullDatapath(metadata.filepath);
		return CreateAssetRef<Texture2D>(fullPath);
	}

	AssetMetadata TextureImporter::GetMetadata() const
	{
		return AssetMetadata{ Texture2D::GetStaticType().data(), m_Path, "", false, GetDeclaredDependencies() };
	}
}
