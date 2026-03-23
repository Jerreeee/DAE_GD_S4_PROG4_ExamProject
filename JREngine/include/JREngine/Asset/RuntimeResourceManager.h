#pragma once
#include <map>
#include <unordered_set>
#include <filesystem>
#include "JREngine/Asset/IResourceManager.h"
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	class RuntimeResourceManager final : public IResourceManager
	{
	public:
		explicit RuntimeResourceManager(const std::filesystem::path& manifestPath);

		bool IsValidAssetHandle(AssetHandle handle) const override;
		bool IsAssetLoaded(AssetHandle handle) const override;
		AssetRef<Asset> GetAsset(AssetHandle handle, AssetLoadMode mode = AssetLoadMode::Unspecified) override;
		AssetHandle AddAsset(AssetRef<Asset> asset) override;

	private:
		AssetRef<Asset> LoadAssetAndDeps(AssetHandle handle,
		                                 std::unordered_set<uint64_t>& visited);

		std::map<AssetHandle, AssetRef<Asset>> m_LoadedAssets;
	};
}
