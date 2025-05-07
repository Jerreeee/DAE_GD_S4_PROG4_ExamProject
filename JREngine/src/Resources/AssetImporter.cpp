#include "Resources/AssetImporter.h"

namespace JRE
{
	void AssetImporter::RegisterImporter(AssetType type, Ref<IAssetImporter> importer)
	{
		GetImporterMap().emplace(type, std::move(importer));
	}

	Ref<IAssetImporter> AssetImporter::GetImporter(AssetType type)
	{
		auto& map = GetImporterMap();
		auto it = map.find(type);
		if (it == map.end())
			throw std::runtime_error("No importer registered for the requested AssetType");
		return it->second;
	}

	std::map<AssetType, Ref<IAssetImporter>>& AssetImporter::GetImporterMap()
	{
		static std::map<AssetType, Ref<IAssetImporter>> s_Importers{};
		return s_Importers;
	}
}
