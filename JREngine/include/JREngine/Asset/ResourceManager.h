#pragma once
#include <concepts>
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Asset/IResourceManager.h"
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	/// <summary>
	/// Static ResourceManager class that provides extra utility functions
	/// </summary>
	class ResourceManager final
	{
	public:
		//Returns the active ResourceManager
		static IResourceManager& GetActive()
		{
			return ServiceLocator::GetResourceManager();
		}

		//Tries to get the asset but wont force load
		template<IsAsset T>
		static AssetRef<T> TryGetAsset(AssetHandle handle, AssetLoadMode loadMode = AssetLoadMode::Unspecified)
		{
			return static_pointer_cast<T>(GetActive().GetAsset(handle, loadMode));
		}

		//Gets the asset, force laods if needed
		template<IsAsset T>
		static AssetRef<T> GetAsset(AssetHandle handle)
		{
			return static_pointer_cast<T>(GetActive().GetAsset(handle, AssetLoadMode::Immediate));
		}

		//Adds an existing asset and assigns it a AssetHandle
		static AssetHandle AddAsset(AssetRef<Asset> asset)
		{
			return GetActive().AddAsset(asset);
		}

		//Creates an asset from args and adds it to the ResourceManager
		template<IsAsset T, typename... Args>
		static AssetHandle CreateAsset(Args&&... args)
		{
			AssetRef<T> asset = JRE::CreateAssetRef<T>(std::forward<Args>(args)...);
			return GetActive().AddAsset(asset);
		}

		//Creates an AssetRef<T> from args and adds the asset to the ResourceManager
		template<IsAsset T, typename... Args>
		static AssetRef<T> CreateAssetRef(Args&&... args)
		{
			AssetRef<T> asset = JRE::CreateAssetRef<T>(std::forward<Args>(args)...);
			GetActive().AddAsset(asset);
			return asset;
		}

		////Creates an AssetRef<T> from args and adds the asset to the ResourceManager
		//template<IsAsset T, typename... Args>
		//static SoftAssetRef<T> CreateSoftAssetRef(Args&&... args)
		//{
		//	auto asset = CreateRef<T>(std::forward<Args>(args)...);
		//	auto handle = GetActive().AddAsset(asset);
		//	return SoftAssetRef<T>(handle, asset);
		//}
	};
}
