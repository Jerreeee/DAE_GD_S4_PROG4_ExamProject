#include "Asset/AssetRegistry.h"
#include "Asset/AssetImporter.h"

namespace JRE
{
	void AssetImporterRegistry::Init(const std::filesystem::path& dataPath)
	{
		m_Datapath = dataPath;
	}
	bool AssetImporterRegistry::RegisterImporter(const std::string& typeName, ImportFunc importFunc)
	{
		auto it = s_Importers.find(typeName);
		if (it != s_Importers.end())
			return false;
		s_Importers.emplace(typeName, importFunc);
		return true;
	}

	AssetHandle AssetImporterRegistry::ImportAsset(IAssetImporter&& importer)
	{
		return AssetRegistry::GetInstance().RegisterAsset(std::move(importer));
	}

	AssetRef<Asset> AssetImporterRegistry::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		auto it = s_Importers.find(metadata.assetType);
		return it != s_Importers.end() ? it->second(handle, metadata) : nullptr;
	}
	const std::filesystem::path& AssetImporterRegistry::GetDatapath() const
	{
		return m_Datapath;
	}
	std::filesystem::path AssetImporterRegistry::GetFullDatapath(const std::filesystem::path& filepath) const
	{
		return std::filesystem::path(m_Datapath / filepath);
	}
}
