#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include "JREngine/Resources/IResourceManager.h"
#include "JREngine/Resources/ResourceHandle.h"

namespace JRE
{
	class ResourceManager final : public IResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		void Init(const std::filesystem::path& data) override;

		ResourceHandle<Texture2D> LoadTexture(const std::string& file) override;
		ResourceHandle<Texture2D> LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle) override;
		ResourceHandle<Font> LoadFont(const std::string& file, uint8_t size) override;
		ResourceHandle<ISoundClip> LoadSound(const std::string& file) override;

		std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D> handle) const override;
		std::shared_ptr<Font> GetFont(ResourceHandle<Font> handle) const override;
		std::shared_ptr<ISoundClip> GetSound(ResourceHandle<ISoundClip> handle) const override;
	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}
