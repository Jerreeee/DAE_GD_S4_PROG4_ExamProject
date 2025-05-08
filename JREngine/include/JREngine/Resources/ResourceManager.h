#pragma once
#include <concepts>
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Resources/IResourceManager.h"
#include "JREngine/Resources/IAssetImporter.h"
#include "JREngine/Resources/Asset.h"

template<typename T>
concept IsAsset = std::derived_from<T, JRE::Asset>;

namespace JRE
{
	/// <summary>
	/// Static ResourceManager class that provides extra utility functions
	/// </summary>
	class ResourceManager final
	{
	public:
		template<IsAsset T>
		static Ref<T> TryGetAssset(AssetHandle handle, AssetLoadMode loadMode = AssetLoadMode::Unspecified)
		{
			return static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(handle, loadMode));
		}

		template<IsAsset T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			return static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(handle, AssetLoadMode::Immediate));
		}

		static AssetHandle AddAsset(Ref<Asset> asset)
		{
			return ServiceLocator::GetResourceManager().AddAsset(asset);
		}
	};
}
