#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "SoundClip.h"
#include "ResourceManager.h"

namespace fs = std::filesystem;

namespace JREngine
{
	void ResourceManager::Init(const std::filesystem::path& dataPath)
	{
		m_dataPath = dataPath;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
		}
	}

	std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file)
	{
		const auto fullPath = m_dataPath / file;
		const auto filename = fs::path(fullPath).filename().string();
		if (m_LoadedTextures.find(filename) == m_LoadedTextures.end())
			m_LoadedTextures.insert(std::pair(filename, std::make_shared<Texture2D>(fullPath.string())));
		return m_LoadedTextures.at(filename);
	}

	std::shared_ptr<Font> ResourceManager::LoadFont(const std::string& file, uint8_t size)
	{
		const auto fullPath = m_dataPath / file;
		const auto filename = fs::path(fullPath).filename().string();
		const auto key = std::pair<std::string, uint8_t>(filename, size);
		if (m_LoadedFonts.find(key) == m_LoadedFonts.end())
			m_LoadedFonts.insert(std::pair(key, std::make_shared<Font>(fullPath.string(), size)));
		return m_LoadedFonts.at(key);
	}

	std::shared_ptr<SoundClip> ResourceManager::LoadSound(const std::string& file)
	{
		const auto fullPath = m_dataPath / file;
		const auto filename = fs::path(fullPath).filename().string();

		auto it = m_LoadedSounds.find(filename);
		if (it == m_LoadedSounds.end())
		{
			auto sound = std::make_shared<SoundClip>(fullPath.string());
			m_LoadedSounds.emplace(filename, sound);
			return sound;
		}
		return it->second;
	}

	void ResourceManager::UnloadUnusedResources()
	{
		for (auto it = m_LoadedTextures.begin(); it != m_LoadedTextures.end();)
		{
			if (it->second.use_count() == 1)
				it = m_LoadedTextures.erase(it);
			else
				++it;
		}

		for (auto it = m_LoadedFonts.begin(); it != m_LoadedFonts.end();)
		{
			if (it->second.use_count() == 1)
				it = m_LoadedFonts.erase(it);
			else
				++it;
		}

		for (auto it = m_LoadedSounds.begin(); it != m_LoadedSounds.end();)
		{
			if (it->second.use_count() == 1)
				it = m_LoadedSounds.erase(it);
			else
				++it;
		}
	}
}