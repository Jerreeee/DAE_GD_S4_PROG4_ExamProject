#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"
#include "ResourceHandle.h"
#include "IResourceManager.h"

namespace JRE
{
	class ResourceManager final : public IResourceManager
	{
	public:
		virtual void Init(const std::filesystem::path& data) override;
		virtual ResourceHandle<Texture2D> LoadTexture(const std::string& file) override;
		virtual ResourceHandle<Texture2D> LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle) override;
		virtual ResourceHandle<Font> LoadFont(const std::string& file, uint8_t size) override;
		virtual ResourceHandle<SoundClip> LoadSound(const std::string& file) override;

		virtual std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D> handle) const override;
		virtual std::shared_ptr<Font> GetFont(ResourceHandle<Font> handle) const override;
		virtual std::shared_ptr<SoundClip> GetSound(ResourceHandle<SoundClip> handle) const override;
	private:
		std::filesystem::path m_dataPath;

		void UnloadUnusedResources();

		std::map<GUID, std::shared_ptr<Texture2D>> m_LoadedTextures{};
		std::map<GUID, std::shared_ptr<Font>> m_LoadedFonts{};
		std::map<GUID, std::shared_ptr<SoundClip>> m_LoadedSounds{};

		std::map<std::filesystem::path, GUID> m_TexturePathToGUID{};
		std::map<std::pair<std::filesystem::path, uint8_t>, GUID> m_FontPathSizeToGUID{};
		std::map<std::filesystem::path, GUID> m_SoundPathToGUID{};
	};
}
