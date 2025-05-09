#pragma once
#include <map>
#include <mutex>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Resources/Asset.h"
#include "JREngine/Resources/IAssetImporter.h"
#include "JREngine/Resources/AssetMetadata.h"

namespace JRE
{
	class AssetRegistry final : public Singleton<AssetRegistry>
	{
	public:
		AssetHandle RegisterAsset(IAssetImporter&& importer);
		bool IsValidAssetHandle(AssetHandle handle) const;
		bool HasAssetAtPath(const std::filesystem::path virtualPath);
		AssetHandle GetHandleAtPath(const std::filesystem::path virtualPath) const;
		const AssetMetadata& GetMetadata(AssetHandle handle) const;
	private:
		AssetRegistry() = default;
		friend class Singleton<AssetRegistry>;

		mutable std::mutex m_Mutex{};
		std::map<AssetHandle, AssetMetadata> m_AssetHandleToMetadata{};
		std::map<std::filesystem::path, AssetHandle> m_PathToAssetHandle{};
	};
}
