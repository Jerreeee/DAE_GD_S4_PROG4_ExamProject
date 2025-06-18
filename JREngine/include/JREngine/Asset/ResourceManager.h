#pragma once
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

		//Adds an existing asset and assigns it an AssetHandle
		static AssetHandle AddAsset(std::unique_ptr<Asset> asset)
		{
			return GetActive().AddAsset(std::move(asset));
		}
	};
}
