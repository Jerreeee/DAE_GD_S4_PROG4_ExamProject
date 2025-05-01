#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

#include "ServiceLocator.h"
#include "ISoundSystem.h"
#include "ISoundClip.h"

#include "ResourceManager.h"

namespace fs = std::filesystem;

namespace JRE
{
	void ResourceManager::Init(const std::filesystem::path& dataPath)
	{
		m_dataPath = dataPath;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
		}

		m_WorkerThread = std::jthread{ [this](std::stop_token token) { WorkerLoop(token); } };
	}

	ResourceHandle<Texture2D> ResourceManager::LoadTexture(const std::string& file)
	{
		const auto fullPath = m_dataPath / file;

		auto it = m_TexturePathToGUID.find(fullPath);
		if (it != m_TexturePathToGUID.end())
			return ResourceHandle<Texture2D>(it->second);

		GUID guid = GenerateGUID();
		m_TexturePathToGUID.emplace(fullPath, guid);

		auto pTexture = std::make_shared<Texture2D>(fullPath.string());
		{
			std::scoped_lock lock(m_Mutex);
			m_LoadedTextures.emplace(guid, pTexture);
		}

		return ResourceHandle<Texture2D>(guid);
	}
	ResourceHandle<Texture2D> ResourceManager::LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle)
	{
		auto pFont = GetFont(fontHandle);
		if (!pFont)
			throw std::runtime_error(std::string("Not a valid font"));

		// Make a fake path based on the text + font GUID
		std::string fakePath = "generated/" + text + "_" + std::to_string(fontHandle.GetGUID());
		std::filesystem::path fakeFilePath(fakePath);

		auto it = m_TexturePathToGUID.find(fakeFilePath);
		if (it != m_TexturePathToGUID.end())
			return ResourceHandle<Texture2D>(it->second);

		GUID guid = GenerateGUID();
		m_TexturePathToGUID.emplace(fakeFilePath, guid);

		auto pTexture = std::make_shared<Texture2D>(text, fontHandle);
		{
			std::scoped_lock lock(m_Mutex);
			m_LoadedTextures.emplace(guid, pTexture);
		}

		return ResourceHandle<Texture2D>(guid);
	}
	ResourceHandle<Font> ResourceManager::LoadFont(const std::string& file, uint8_t size)
	{
		const auto fullPath = m_dataPath / file;
		auto key = std::make_pair(fullPath, size);

		auto it = m_FontPathSizeToGUID.find(key);
		if (it != m_FontPathSizeToGUID.end())
			return ResourceHandle<Font>(it->second);

		GUID guid = GenerateGUID();
		m_FontPathSizeToGUID.emplace(key, guid);

		EnqueueLoadEvent(CreateEvent<LoadEvents::LoadFont>(file, size, guid));

		return ResourceHandle<Font>(guid);
	}

	ResourceHandle<ISoundClip> ResourceManager::LoadSound(const std::string& file)
	{
		const auto fullPath = m_dataPath / file;

		auto it = m_SoundPathToGUID.find(fullPath);
		if (it != m_SoundPathToGUID.end())
			return ResourceHandle<ISoundClip>(it->second);

		GUID guid = GenerateGUID();
		m_SoundPathToGUID.emplace(fullPath, guid);

		EnqueueLoadEvent(CreateEvent<LoadEvents::LoadSound>(file, guid));

		return ResourceHandle<ISoundClip>(guid);
	}
	std::shared_ptr<Texture2D> ResourceManager::GetTexture(ResourceHandle<Texture2D> handle) const
	{
		auto it = m_LoadedTextures.find(handle.GetGUID());
		if (it != m_LoadedTextures.end())
			return it->second;
		else
			return nullptr;
	}

	std::shared_ptr<Font> ResourceManager::GetFont(ResourceHandle<Font> handle) const
	{
		auto it = m_LoadedFonts.find(handle.GetGUID());
		if (it != m_LoadedFonts.end())
			return it->second;
		else
			return nullptr;
	}

	std::shared_ptr<ISoundClip> ResourceManager::GetSound(ResourceHandle<ISoundClip> handle) const
	{
		auto it = m_LoadedSounds.find(handle.GetGUID());
		if (it != m_LoadedSounds.end())
			return it->second;
		else
			return nullptr;
	}
	void ResourceManager::WorkerLoop(std::stop_token token)
	{
		std::unique_lock lock(m_Mutex);

		while (!token.stop_requested())
		{
			m_Condition.wait(lock, token, [&] { return !m_LoadEventsQueue.empty(); });

			while (!m_LoadEventsQueue.empty())
			{
				EventInfo event = std::move(m_LoadEventsQueue.front());
				m_LoadEventsQueue.pop();

				lock.unlock();

				switch (event.GetID())
				{
				case LoadEvents::LoadFont::ID:
				{
					auto& args = event.GetArgs<LoadEvents::LoadFont>();
					auto font = std::make_shared<Font>((m_dataPath / args.path).string(), args.size);
					std::lock_guard g(m_Mutex);
					m_LoadedFonts[args.guid] = font;
					break;
				}
				case LoadEvents::LoadSound::ID:
				{
					auto& args = event.GetArgs<LoadEvents::LoadSound>();
					auto sound = ServiceLocator::GetSoundSystem().CreateSoundClip((m_dataPath / args.path).string());
					std::lock_guard g(m_Mutex);
					m_LoadedSounds[args.guid] = sound;
					break;
				}
				}
				//lock again for next iteration of while loop to safely check "m_SoundQueue.empty()"
				lock.lock();
			}
		}
	}
	void ResourceManager::EnqueueLoadEvent(EventInfo&& event)
	{
		std::scoped_lock lock(m_Mutex);
		m_LoadEventsQueue.push(std::move(event));
		m_Condition.notify_one();
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