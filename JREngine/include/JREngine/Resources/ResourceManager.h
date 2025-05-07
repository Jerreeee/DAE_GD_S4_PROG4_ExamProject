#pragma once
#include <concepts>
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Resources/IResourceManager.h"
#include "JREngine/Resources/IAssetImporter.h"
#include "JREngine/Resources/Asset.h"

template<typename T>
concept IsAsset = std::derived_from<T, JRE::Asset>;

template<typename T, typename... Args>
concept CreatableAsset = IsAsset<T> &&
std::constructible_from<T, Args...>&&
	requires(Args&&... args) {
		{ T::PathBuilder::Build(std::forward<Args>(args)...) } -> std::same_as<std::filesystem::path>;
};

namespace JRE
{
	/// <summary>
	/// Static ResourceManager class that provides extra utility functions
	/// </summary>
	class ResourceManager final
	{
	public:
		static IResourceManager& GetActive()
		{
			return ServiceLocator::GetResourceManager();
		}

		template<IsAsset T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			return static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(handle));
		}

		template<IsAsset T>
		static Ref<T> GetAsset(const std::filesystem::path& path)
		{
			return static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(path));
		}

		template<IsAsset T>
		static AssetHandle LoadAsset(const std::filesystem::path& path,
			std::unique_ptr<IAssetSpecificImportSettings>&& pSettings = nullptr,
			AssetLoadMode loadMode = AssetLoadMode::Immediate)
		{
			return ServiceLocator::GetResourceManager().LoadAsset(path, T::GetStaticType(), std::move(pSettings), loadMode);
		}

		template<typename T, typename... Args>
		requires CreatableAsset<T, Args...>
		AssetHandle CreateAsset(Args... args)
		{
			auto path = T::PathBuilder(std::forward<Args>(args)...);
			if (GetActive().IsAssetLoaded(path))
				return GetAsset<T>(path)->GetHandle();
			auto asset = CreateRef<T>(std::forward<Args>(args)...);
			GetActive().AddAsset(asset, path);
			return asset->GetHandle();
		}
	};
}
