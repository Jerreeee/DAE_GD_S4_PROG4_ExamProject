#pragma once
#include <memory>
#include <string>
#include <filesystem>
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

		virtual AssetHandle LoadTexture(const std::string& path) = 0;
		virtual AssetHandle LoadTexture(const std::string& text, AssetHandle fontHandle) = 0;
		virtual AssetHandle LoadFont(const std::string& path, uint8_t size) = 0;
		virtual AssetHandle LoadSound(const std::string& path) = 0;

		virtual Ref<Asset> GetAsset(AssetHandle handle) const = 0;
	};

	class NullResourceManager final : public IResourceManager
	{
	public:
		void Init(const std::filesystem::path&) override {}

		virtual AssetHandle LoadTexture(const std::string&) override { return AssetHandle::InvalidUUID; }
		virtual AssetHandle LoadTexture(const std::string&, AssetHandle) override { return AssetHandle::InvalidUUID; }
		virtual AssetHandle LoadFont(const std::string&, uint8_t) override { return AssetHandle::InvalidUUID; }
		virtual AssetHandle LoadSound(const std::string&) override { return AssetHandle::InvalidUUID; }

		virtual Ref<Asset> GetAsset(AssetHandle) const override { return nullptr; };
	};
}