#pragma once
#include <memory>
#include <string>
#include <filesystem>
#include <functional>
#include "JREngine/Resources/IAssetImporter.h"
#include "JREngine/Resources/Asset.h"

namespace JRE
{
	class Texture2D;
	class Font;
	class ISoundClip;

	class IResourceManager
	{
	public:
		virtual ~IResourceManager() = default;

		virtual void Init(const std::filesystem::path& data) = 0;

		//The asset's AssetHandle MUST be Assethandle::InvalidUUID to succesfully add the asset
		//If the asset was already added before its handle is returned
		virtual AssetHandle AddAsset(Ref<Asset> asset, const std::filesystem::path& path) = 0;
		//Will import the asset and call AddAsset()
		virtual AssetHandle LoadAsset(const std::filesystem::path& path, AssetType assetType,
			std::unique_ptr<IAssetSpecificImportSettings>&& pSettings = nullptr,
			AssetLoadMode loadMode = AssetLoadMode::Immediate) = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(const std::filesystem::path& path) const = 0;
		virtual Ref<Asset> GetAsset(AssetHandle handle) const = 0;
		virtual Ref<Asset> GetAsset(const std::filesystem::path& path) const = 0;
	};

	class NullResourceManager final : public IResourceManager
	{
	public:
		void Init(const std::filesystem::path&) override {}

		virtual AssetHandle AddAsset(Ref<Asset>, const std::filesystem::path&) override { return AssetHandle::InvalidUUID; };
		virtual AssetHandle LoadAsset(const std::filesystem::path&, AssetType,
			std::unique_ptr<IAssetSpecificImportSettings>&&,
			AssetLoadMode) override { return AssetHandle::InvalidUUID; };
		virtual bool IsAssetLoaded(AssetHandle) const override { return false; };
		virtual bool IsAssetLoaded(const std::filesystem::path&) const override { return false; };
		virtual Ref<Asset> GetAsset(AssetHandle) const override { return nullptr; };
		virtual Ref<Asset> GetAsset(const std::filesystem::path&) const override { return nullptr; };
	};
}
