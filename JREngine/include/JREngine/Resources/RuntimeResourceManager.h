#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Resources/IResourceManager.h"
#include "JREngine/Resources/Asset.h"

namespace JRE
{
	class RuntimeResourceManager final : public IResourceManager
	{
	public:
		RuntimeResourceManager();
		~RuntimeResourceManager();

		virtual void Init(const std::filesystem::path& data) override;

		virtual AssetHandle AddAsset(Ref<Asset> asset, const std::filesystem::path& path) override;
		//Real RuntimeAssetManager should probably not take in ImportSettings because it should load
		//assets from Assetpacks but currently this "Runtime"ResourceManager does everything
		//including loading form disk so it does need the ImportSettings
		virtual AssetHandle LoadAsset(const std::filesystem::path& path, AssetType assetType,
			std::unique_ptr<IAssetSpecificImportSettings>&& pSettings = nullptr,
			AssetLoadMode loadMode = AssetLoadMode::Immediate) override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(const std::filesystem::path& path) const override;
		virtual Ref<Asset> GetAsset(AssetHandle handle) const override;
		virtual Ref<Asset> GetAsset(const std::filesystem::path& path) const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}
