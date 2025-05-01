#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <iostream>

#include <SDL_ttf.h>

#include "Texture2D.h"
#include "Font.h"
#include "ISoundClip.h"
#include "ISoundSystem.h"
#include "ServiceLocator.h"

#include "Event.h"
#include "ResourceManager.h"

namespace JRE
{
	class ResourceManager::Impl
	{
	public:
		Impl();

		void Init(const std::filesystem::path& dataPath);
		ResourceHandle<Texture2D> LoadTexture(const std::string& file);
		ResourceHandle<Texture2D> LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle);
		ResourceHandle<Font> LoadFont(const std::string& file, uint8_t size);
		ResourceHandle<ISoundClip> LoadSound(const std::string& file);

		std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D> handle) const;
		std::shared_ptr<Font> GetFont(ResourceHandle<Font> handle) const;
		std::shared_ptr<ISoundClip> GetSound(ResourceHandle<ISoundClip> handle) const;
	private:
		struct LoadEvents
		{
			struct LoadFont
			{
				static const EventID ID{ HashEventID("LoadFont") };
				struct Args : public EventArgs
				{
					Args(std::string _path, uint8_t _size, GUID _guid)
						: path(std::move(_path)), size(_size), guid(_guid) {
					}

					std::string path;
					uint8_t size;
					GUID guid;
				};
			};

			struct LoadSound
			{
				static const EventID ID{ HashEventID("LoadSound") };
				struct Args : public EventArgs
				{
					Args(std::string _path, GUID _guid)
						: path(std::move(_path)), guid(_guid) {
					}

					std::string path;
					GUID guid;
				};
			};
		};

		void WorkerLoop(std::stop_token token);
		void EnqueueLoadEvent(EventInfo&& event);

		std::filesystem::path m_DataPath{};

		std::map<GUID, std::shared_ptr<Texture2D>> m_LoadedTextures{};
		std::map<GUID, std::shared_ptr<Font>> m_LoadedFonts{};
		std::map<GUID, std::shared_ptr<ISoundClip>> m_LoadedSounds{};

		std::map<std::filesystem::path, GUID> m_TexturePathToGUID{};
		std::map<std::pair<std::filesystem::path, uint8_t>, GUID> m_FontPathSizeToGUID{};
		std::map<std::filesystem::path, GUID> m_SoundPathToGUID{};

		std::queue<EventInfo> m_LoadEventsQueue{};
		mutable std::mutex m_Mutex{};
		std::condition_variable_any m_Condition{};
		std::jthread m_WorkerThread{};
	};
	ResourceManager::Impl::Impl() = default;
	void ResourceManager::Impl::Init(const std::filesystem::path& dataPath)
	{
		m_DataPath = dataPath;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error("Failed to initialize TTF: " + std::string(SDL_GetError()));
		}

		m_WorkerThread = std::jthread([this](std::stop_token token) { WorkerLoop(token); });
	}
	ResourceHandle<Texture2D> ResourceManager::Impl::LoadTexture(const std::string& file)
	{
		auto fullPath = m_DataPath / file;
		if (auto it = m_TexturePathToGUID.find(fullPath); it != m_TexturePathToGUID.end())
			return ResourceHandle<Texture2D>(it->second);

		GUID guid = GenerateGUID();
		m_TexturePathToGUID[fullPath] = guid;
		auto tex = std::make_shared<Texture2D>(fullPath.string());
		{
			std::scoped_lock lock(m_Mutex);
			m_LoadedTextures[guid] = tex;
		}
		return ResourceHandle<Texture2D>(guid);
	}
	ResourceHandle<Texture2D> ResourceManager::Impl::LoadTexture(const std::string& text, ResourceHandle<Font> fontHandle)
	{
		auto font = GetFont(fontHandle);
		if (!font) throw std::runtime_error("Invalid font handle");
		auto fakePath = std::filesystem::path("generated/" + text + "_" + std::to_string(fontHandle.GetGUID()));

		if (auto it = m_TexturePathToGUID.find(fakePath); it != m_TexturePathToGUID.end())
			return ResourceHandle<Texture2D>(it->second);

		GUID guid = GenerateGUID();
		m_TexturePathToGUID[fakePath] = guid;
		auto tex = std::make_shared<Texture2D>(text, fontHandle);
		{
			std::scoped_lock lock(m_Mutex);
			m_LoadedTextures[guid] = tex;
		}
		return ResourceHandle<Texture2D>(guid);
	}
	ResourceHandle<Font> ResourceManager::Impl::LoadFont(const std::string& file, uint8_t size)
	{
		auto fullPath = m_DataPath / file;
		auto key = std::make_pair(fullPath, size);

		if (auto it = m_FontPathSizeToGUID.find(key); it != m_FontPathSizeToGUID.end())
			return ResourceHandle<Font>(it->second);

		GUID guid = GenerateGUID();
		m_FontPathSizeToGUID[key] = guid;
		EnqueueLoadEvent(CreateEvent<LoadEvents::LoadFont>(file, size, guid));
		return ResourceHandle<Font>(guid);
	}
	ResourceHandle<ISoundClip> ResourceManager::Impl::LoadSound(const std::string& file)
	{
		auto fullPath = m_DataPath / file;
		if (auto it = m_SoundPathToGUID.find(fullPath); it != m_SoundPathToGUID.end())
			return ResourceHandle<ISoundClip>(it->second);

		GUID guid = GenerateGUID();
		m_SoundPathToGUID[fullPath] = guid;
		EnqueueLoadEvent(CreateEvent<LoadEvents::LoadSound>(file, guid));
		return ResourceHandle<ISoundClip>(guid);
	}
	std::shared_ptr<Texture2D> ResourceManager::Impl::GetTexture(ResourceHandle<Texture2D> handle) const
	{
		auto it = m_LoadedTextures.find(handle.GetGUID());
		return (it != m_LoadedTextures.end()) ? it->second : nullptr;
	}

	std::shared_ptr<Font> ResourceManager::Impl::GetFont(ResourceHandle<Font> handle) const
	{
		auto it = m_LoadedFonts.find(handle.GetGUID());
		return (it != m_LoadedFonts.end()) ? it->second : nullptr;
	}
	std::shared_ptr<ISoundClip> ResourceManager::Impl::GetSound(ResourceHandle<ISoundClip> handle) const
	{
		auto it = m_LoadedSounds.find(handle.GetGUID());
		return (it != m_LoadedSounds.end()) ? it->second : nullptr;
	}
	void ResourceManager::Impl::EnqueueLoadEvent(EventInfo&& event)
	{
		std::scoped_lock lock(m_Mutex);
		m_LoadEventsQueue.push(std::move(event));
		m_Condition.notify_one();
	}
	void ResourceManager::Impl::WorkerLoop(std::stop_token token)
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
					auto font = std::make_shared<Font>((m_DataPath / args.path).string(), args.size);
					std::scoped_lock g(m_Mutex);
					m_LoadedFonts[args.guid] = font;
					break;
				}
				case LoadEvents::LoadSound::ID:
				{
					auto& args = event.GetArgs<LoadEvents::LoadSound>();
					auto sound = ServiceLocator::GetSoundSystem().CreateSoundClip((m_DataPath / args.path).string());
					std::scoped_lock g(m_Mutex);
					m_LoadedSounds[args.guid] = sound;
					break;
				}
				default:
					std::cerr << "Unknown event ID in ResourceManager\n";
					break;
				}
				lock.lock();
			}
		}
	}

	ResourceManager::ResourceManager() : m_pImpl{ std::make_unique<Impl>() } {}
	ResourceManager::~ResourceManager() = default;
	void ResourceManager::Init(const std::filesystem::path& data) { m_pImpl->Init(data); }
	ResourceHandle<Texture2D> ResourceManager::LoadTexture(const std::string& file) { return m_pImpl->LoadTexture(file); }
	ResourceHandle<Texture2D> ResourceManager::LoadTexture(const std::string& text, ResourceHandle<Font> font) { return m_pImpl->LoadTexture(text, font); }
	ResourceHandle<Font> ResourceManager::LoadFont(const std::string& file, uint8_t size) { return m_pImpl->LoadFont(file, size); }
	ResourceHandle<ISoundClip> ResourceManager::LoadSound(const std::string& file) { return m_pImpl->LoadSound(file); }
	std::shared_ptr<Texture2D> ResourceManager::GetTexture(ResourceHandle<Texture2D> handle) const { return m_pImpl->GetTexture(handle); }
	std::shared_ptr<Font> ResourceManager::GetFont(ResourceHandle<Font> handle) const { return m_pImpl->GetFont(handle); }
	std::shared_ptr<ISoundClip> ResourceManager::GetSound(ResourceHandle<ISoundClip> handle) const { return m_pImpl->GetSound(handle); }
}
