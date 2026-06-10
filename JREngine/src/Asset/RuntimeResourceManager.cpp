#include <fstream>
#include <stdexcept>
#include "Asset/RuntimeResourceManager.h"
#include "Asset/AssetRegistry.h"
#include "Asset/AssetLoaderRegistry.h"
#include "JREngine/Core/UUID.h"

namespace JRE
{
	RuntimeResourceManager::RuntimeResourceManager(const std::filesystem::path& manifestPath)
	{
		// The data folder is the manifest's parent (Data/asset_manifest.txt). Importers resolve
		// their relative paths via AssetRegistry::GetFullDatapath() at load time, so the registry
		// needs to know where Data/ is. Mirrors AssetDatabase::Init() on the editor side.
		AssetRegistry::GetInstance().SetDataPath(manifestPath.parent_path());

		std::ifstream in(manifestPath);
		if (!in)
			throw std::runtime_error("RuntimeResourceManager: asset_manifest.txt not found");
		if (!AssetRegistry::GetInstance().Deserialize(in))
			throw std::runtime_error("RuntimeResourceManager: failed to parse asset_manifest.txt");
	}

	bool RuntimeResourceManager::IsValidAssetHandle(AssetHandle handle) const
	{
		return AssetRegistry::GetInstance().IsValidAssetHandle(handle);
	}

	bool RuntimeResourceManager::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.contains(handle);
	}

	AssetRef<Asset> RuntimeResourceManager::GetAsset(AssetHandle handle, AssetLoadMode /*mode*/)
	{
		auto it = m_LoadedAssets.find(handle);
		if (it != m_LoadedAssets.end())
			return it->second;

		if (!AssetRegistry::GetInstance().IsValidAssetHandle(handle))
			return nullptr;

		std::unordered_set<uint64_t> visited;
		return LoadAssetAndDeps(handle, visited);
	}

	AssetRef<Asset> RuntimeResourceManager::LoadAssetAndDeps(AssetHandle handle,
	                                                          std::unordered_set<uint64_t>& visited)
	{
		if (!visited.insert(static_cast<uint64_t>(handle)).second)
			throw std::runtime_error("RuntimeResourceManager: circular asset dependency detected");

		// Already cached from a previous dep path
		auto it = m_LoadedAssets.find(handle);
		if (it != m_LoadedAssets.end())
		{
			visited.erase(static_cast<uint64_t>(handle));
			return it->second;
		}

		const AssetMetadata& meta = AssetRegistry::GetInstance().GetMetadata(handle);

		// Load dependencies depth-first
		for (AssetHandle dep : meta.dependencies)
			LoadAssetAndDeps(dep, visited);

		AssetRef<Asset> asset = AssetLoaderRegistry::GetInstance().Load(handle, meta);
		if (asset)
		{
			asset->SetHandle(handle);
			m_LoadedAssets.emplace(handle, asset);
		}

		visited.erase(static_cast<uint64_t>(handle));
		return asset;
	}

	AssetHandle RuntimeResourceManager::AddAsset(AssetRef<Asset> asset)
	{
		AssetHandle handle = UUID::Generate();
		asset->SetHandle(handle);
		m_LoadedAssets.emplace(handle, asset);
		return handle;
	}
}
