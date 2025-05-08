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
		virtual bool IsValidAssetHandle(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;
		virtual Ref<Asset> GetAsset(AssetHandle handle, AssetLoadMode loadMode = AssetLoadMode::Unspecified) = 0;
		virtual AssetHandle AddAsset(Ref<Asset> asset) = 0;
	};

	class NullResourceManager final : public IResourceManager
	{
	public:
		virtual bool IsValidAssetHandle(AssetHandle) const override { return false; };
		virtual bool IsAssetLoaded(AssetHandle) const override { return false; };
		virtual Ref<Asset> GetAsset(AssetHandle, AssetLoadMode) override { return nullptr; };
		virtual AssetHandle AddAsset(Ref<Asset> asset) override { return AssetHandle::InvalidUUID; };
	};
}
