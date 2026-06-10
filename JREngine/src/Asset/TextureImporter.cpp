#include "Asset/AssetRegistry.h"
#include "Asset/TextureImporter.h"

namespace JRE
{
	// Loader registration is done explicitly at engine init (see RegisterBuiltinAssetLoaders
	// in JREngine.cpp), not via a static initializer here — those get stripped from the static lib.

	TextureImporter::TextureImporter(const std::filesystem::path& filepath) :
		m_Path{ filepath }
	{
	}

	AssetRef<Asset> TextureImporter::Load(AssetHandle, const AssetMetadata& metadata)
	{
		auto fullPath = AssetRegistry::GetInstance().GetFullDatapath(metadata.filepath);
		return CreateAssetRef<Texture2D>(fullPath);
	}

	AssetMetadata TextureImporter::GetMetadata() const
	{
		return AssetMetadata{ Texture2D::GetStaticType().data(), m_Path, "", false, GetDeclaredDependencies() };
	}
}
