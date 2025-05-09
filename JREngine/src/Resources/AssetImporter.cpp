#include "Resources/AssetRegistry.h"
#include "Resources/AssetImporter.h"

namespace JRE
{
	void AssetImporter::Init(const std::filesystem::path& dataPath)
	{
		m_Datapath = dataPath;
	}
	void AssetImporter::RegisterImporter(AssetType type, ImportFunc importFunc)
	{
		s_Importers.emplace(type, importFunc);
	}

	AssetHandle AssetImporter::ImportAsset(IAssetImporter&& importer)
	{
		return AssetRegistry::GetInstance().RegisterAsset(std::move(importer));
	}

	Ref<Asset> AssetImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		auto it = s_Importers.find(metadata.assetType);
		return it != s_Importers.end() ? it->second(handle, metadata) : nullptr;
	}
	const std::filesystem::path& AssetImporter::GetDatapath() const
	{
		return m_Datapath;
	}
	std::filesystem::path AssetImporter::GetFullDatapath(const std::filesystem::path& filepath) const
	{
		return std::filesystem::path(m_Datapath / filepath);
	}
}
