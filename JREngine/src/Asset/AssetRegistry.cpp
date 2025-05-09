#include "Asset/AssetRegistry.h"

namespace JRE
{
	AssetHandle AssetRegistry::RegisterAsset(IAssetImporter&& importer)
	{
		AssetMetadata metadata = importer.GetMetadata();

		//Check if the asset is already in the AssetRegistry, ifso return the existing handle
		AssetHandle existingHandle = GetHandleAtPath(metadata.GetVirtualPath());
		if (existingHandle.IsValid())
			return existingHandle;

		//Generate a new AssetHandle
		AssetHandle handle{};
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_AssetHandleToMetadata.emplace(handle, metadata);
		m_PathToAssetHandle.emplace(metadata.GetVirtualPath(), handle);
		return handle;
	}
	bool AssetRegistry::IsValidAssetHandle(AssetHandle handle) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_AssetHandleToMetadata.find(handle) != m_AssetHandleToMetadata.end();
	}
	bool AssetRegistry::HasAssetAtPath(const std::filesystem::path virtualPath)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_PathToAssetHandle.find(virtualPath) != m_PathToAssetHandle.end();
	}
	AssetHandle AssetRegistry::GetHandleAtPath(const std::filesystem::path virtualPath) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		auto it = m_PathToAssetHandle.find(virtualPath);
		return it != m_PathToAssetHandle.end() ? it->second : AssetHandle::InvalidUUID;
	}
	const AssetMetadata& AssetRegistry::GetMetadata(AssetHandle handle) const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		static AssetMetadata nullMetadata;
		auto it = m_AssetHandleToMetadata.find(handle);
		return it != m_AssetHandleToMetadata.end() ? it->second : nullMetadata;
	}
}
