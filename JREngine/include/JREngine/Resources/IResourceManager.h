#pragma once
#include <memory>
#include <string>
#include <filesystem>
#include "JREngine/Resources/ResourceHandle.h"

namespace JRE
{
	class Texture2D;
	class Font;
	class ISoundClip;

	class IResourceManager
	{
	public:
		virtual void Init(const std::filesystem::path& data) = 0;
		virtual ~IResourceManager() = default;

		virtual ResourceHandle<Texture2D> LoadTexture(const std::string& path) = 0;
		virtual ResourceHandle<Texture2D> LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle) = 0;
		virtual ResourceHandle<Font> LoadFont(const std::string& path, uint8_t size) = 0;
		virtual ResourceHandle<ISoundClip> LoadSound(const std::string& path) = 0;

		//template<typename Resource_t>
		//virtual std::shared_ptr<Resource_t> GetResource(ResourceHandle<Resource_t> handle) const = 0;

		virtual std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D> handle) const = 0;
		virtual std::shared_ptr<Font> GetFont(ResourceHandle<Font> handle) const = 0;
		virtual std::shared_ptr<ISoundClip> GetSound(ResourceHandle<ISoundClip> handle) const = 0;
	};

	class NullResourceManager final : public IResourceManager
	{
	public:
		virtual void Init(const std::filesystem::path&) override {};

		virtual ResourceHandle<Texture2D> LoadTexture(const std::string&) override { return {}; }
		virtual ResourceHandle<Texture2D> LoadTexture(const std::string&, ResourceHandle<Font>) override { return {}; }
		virtual ResourceHandle<Font> LoadFont(const std::string&, uint8_t) override { return {}; }
		virtual ResourceHandle<ISoundClip> LoadSound(const std::string&) override { return {}; }

		virtual std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D>) const override { return nullptr; }
		virtual std::shared_ptr<Font> GetFont(ResourceHandle<Font>) const override { return nullptr; }
		virtual std::shared_ptr<ISoundClip> GetSound(ResourceHandle<ISoundClip>) const override { return nullptr; }
	};
}