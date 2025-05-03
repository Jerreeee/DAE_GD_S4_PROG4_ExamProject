#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Resources/IResourceManager.h"
#include "JREngine/Resources/ResourceHandle.h"

namespace JRE
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		~ResourceManager();

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		void Init(const std::filesystem::path& data);

		ResourceHandle<Texture2D> LoadTexture(const std::string& file);
		ResourceHandle<Texture2D> LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle);
		ResourceHandle<Font> LoadFont(const std::string& file, uint8_t size);
		ResourceHandle<ISoundClip> LoadSound(const std::string& file);

		//template<typename Resource_t>
		//std::shared_ptr<Resource_t> GetResource(ResourceHandle<Resource_t> handle) const
		//{

		//}

		std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D> handle) const;
		std::shared_ptr<Font> GetFont(ResourceHandle<Font> handle) const;
		std::shared_ptr<ISoundClip> GetSound(ResourceHandle<ISoundClip> handle) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager();

		class Impl;
		std::unique_ptr<Impl> m_pImpl{};
	};
}
