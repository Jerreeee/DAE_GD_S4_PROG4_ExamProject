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

#include "Rendering/Texture2D.h"
#include "Resources/Font.h"
#include "Audio/ISoundClip.h"
#include "Audio/ISoundSystem.h"
#include "Core/ServiceLocator.h"

#include "Core/Event.h"
#include "Resources/ResourceManager.h"

namespace JRE
{
	class ResourceManager::Impl
	{
	public:
		Impl();

		void Init(const std::filesystem::path& dataPath);
		AssetHandle LoadTexture(const std::string& file);
		AssetHandle LoadTexture(const std::string& text, AssetHandle fontHandle);
		AssetHandle LoadFont(const std::string& file, uint8_t size);
		AssetHandle LoadSound(const std::string& file);

		Ref<Asset> GetAsset(AssetHandle handle);
	private:
		struct LoadEvents
		{
			struct LoadFont
			{
				static const EventID ID{ HashEventID("LoadFont") };
				struct Args : public EventArgs
				{
					Args(std::string _path, uint8_t _size, AssetHandle _handle)
						: path(std::move(_path)), size(_size), handle(_handle) {}

					std::string path;
					uint8_t size;
					AssetHandle handle;
				};
			};

			struct LoadSound
			{
				static const EventID ID{ HashEventID("LoadSound") };
				struct Args : public EventArgs
				{
					Args(std::string _path, AssetHandle _handle)
						: path(std::move(_path)), handle(_handle) {}

					std::string path;
					AssetHandle handle;
				};
			};
		};

		void WorkerLoop(std::stop_token token);
		void EnqueueLoadEvent(EventInfo&& event);

		std::filesystem::path m_DataPath{};

		std::map<AssetHandle, Ref<Asset>> m_Assets{};
		std::map<std::filesystem::path, AssetHandle> m_PathToAssetHandle{};

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
	AssetHandle ResourceManager::Impl::LoadTexture(const std::string& file)
	{
		auto fullPath = m_DataPath / file;
		if (auto it = m_PathToAssetHandle.find(fullPath); it != m_PathToAssetHandle.end())
			return it->second;

		auto tex = std::make_shared<Texture2D>(fullPath.string());
		m_PathToAssetHandle[fullPath] = tex->GetHandle();
		{
			std::scoped_lock lock(m_Mutex);
			m_Assets[tex->GetHandle()] = tex;
		}
		return tex->GetHandle();
	}
	AssetHandle ResourceManager::Impl::LoadTexture(const std::string& text, AssetHandle fontHandle)
	{
		auto font = GetAsset(fontHandle);
		if (!font) throw std::runtime_error("Invalid font handle");
		auto fakePath = std::filesystem::path("generated/" + text + "@" + std::to_string(uint64_t(fontHandle)));

		if (auto it = m_PathToAssetHandle.find(fakePath); it != m_PathToAssetHandle.end())
			return it->second;

		auto tex = std::make_shared<Texture2D>(text, fontHandle);
		m_PathToAssetHandle[fakePath] = tex->GetHandle();
		{
			std::scoped_lock lock(m_Mutex);
			m_Assets[tex->GetHandle()] = tex;
		}
		return tex->GetHandle();
	}
	AssetHandle ResourceManager::Impl::LoadFont(const std::string& file, uint8_t size)
	{
		auto fullPath = m_DataPath / file;
		std::filesystem::path fakePath{ fullPath.filename().string() + "@" + std::to_string(size) };

		if (auto it = m_PathToAssetHandle.find(fakePath); it != m_PathToAssetHandle.end())
			return it->second;

		AssetHandle handle{};
		m_PathToAssetHandle.emplace(fullPath, handle);
		EnqueueLoadEvent(CreateEvent<LoadEvents::LoadFont>(file, size, handle));
		return handle;
	}
	AssetHandle ResourceManager::Impl::LoadSound(const std::string& file)
	{
		auto fullPath = m_DataPath / file;
		if (auto it = m_PathToAssetHandle.find(fullPath); it != m_PathToAssetHandle.end())
			return it->second;

		AssetHandle handle{};
		m_PathToAssetHandle.emplace(fullPath, handle);
		EnqueueLoadEvent(CreateEvent<LoadEvents::LoadSound>(file, handle));
		return handle;
	}
	Ref<Asset> ResourceManager::Impl::GetAsset(AssetHandle handle)
	{
		auto it = m_Assets.find(handle);
		return it != m_Assets.end() ? it->second : nullptr;
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
					auto font = std::make_shared<Font>((m_DataPath / args.path).string(), args.size, args.handle);
					font->SetHandle(args.handle);
					std::scoped_lock g(m_Mutex);
					m_Assets.emplace(args.handle, font);
					break;
				}
				case LoadEvents::LoadSound::ID:
				{
					auto& args = event.GetArgs<LoadEvents::LoadSound>();
					auto sound = ServiceLocator::GetSoundSystem().CreateSoundClip((m_DataPath / args.path).string(), args.handle);
					sound->SetHandle(args.handle);
					std::scoped_lock g(m_Mutex);
					m_Assets.emplace(args.handle, sound);
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
	AssetHandle ResourceManager::LoadTexture(const std::string& file) { return m_pImpl->LoadTexture(file); }
	AssetHandle ResourceManager::LoadTexture(const std::string& text, AssetHandle fontHandle) { return m_pImpl->LoadTexture(text, fontHandle); }
	AssetHandle ResourceManager::LoadFont(const std::string& file, uint8_t size) { return m_pImpl->LoadFont(file, size); }
	AssetHandle ResourceManager::LoadSound(const std::string& file) { return m_pImpl->LoadSound(file); }
	Ref<Asset> ResourceManager::GetAsset(AssetHandle handle) const { return m_pImpl->GetAsset(handle); }
}
