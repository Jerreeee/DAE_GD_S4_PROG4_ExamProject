#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Resources/IResourceManager.h"
#include "JREngine/Resources/Asset.h"

namespace JRE
{
	class ResourceManager final : public IResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			return static_pointer_cast<T>(ServiceLocator::GetResourceManager().GetAsset(handle));
		}

		virtual void Init(const std::filesystem::path& data) override;

		AssetHandle LoadTexture(const std::string& file) override;
		AssetHandle LoadTexture(const std::string& text, AssetHandle fontHandle) override;
		AssetHandle LoadFont(const std::string& file, uint8_t size) override;
		AssetHandle LoadSound(const std::string& file) override;

		virtual Ref<Asset> GetAsset(AssetHandle handle) const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}
