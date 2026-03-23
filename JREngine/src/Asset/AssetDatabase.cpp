#include <fstream>
#include <stdexcept>
#include "Asset/AssetRegistry.h"
#include "Asset/AssetDatabase.h"

namespace JRE
{
	void AssetDatabase::Init(const std::filesystem::path& dataPath)
	{
		AssetRegistry::GetInstance().SetDataPath(dataPath);
	}

	AssetHandle AssetDatabase::RegisterAsset(IAssetImporter&& importer)
	{
		return AssetRegistry::GetInstance().Register(importer.GetMetadata());
	}

	void AssetDatabase::SerializeManifest(const std::filesystem::path& manifestPath) const
	{
		std::ofstream out(manifestPath);
		if (!out)
			throw std::runtime_error("AssetDatabase: cannot write asset_manifest.txt");
		AssetRegistry::GetInstance().Serialize(out);
	}

}
