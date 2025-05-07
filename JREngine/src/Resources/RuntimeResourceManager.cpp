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
#include <functional>

#include <SDL_ttf.h>

#include "Rendering/Texture2D.h"
#include "Resources/Font.h"
#include "Audio/ISoundClip.h"
#include "Audio/ISoundSystem.h"
#include "Core/ServiceLocator.h"

#include "Core/Event.h"
#include "Resources/AssetImporter.h"
#include "Resources/RuntimeResourceManager.h"

namespace JRE
{
	class RuntimeResourceManager::Impl
	{
	public:
		Impl();

		void Init(const std::filesystem::path& dataPath);

		AssetHandle AddAsset(Ref<Asset> asset, const std::filesystem::path path);
		AssetHandle LoadAsset(const std::filesystem::path& path, AssetType assetType,
			std::unique_ptr<IAssetSpecificImportSettings>&& pSettings = nullptr,
			AssetLoadMode loadMode = AssetLoadMode::Immediate);
		bool IsAssetLoaded(AssetHandle handle) const;
		bool IsAssetLoaded(const std::filesystem::path& path) const;
		Ref<Asset> GetAsset(AssetHandle handle) const;
		Ref<Asset> GetAsset(const std::filesystem::path& path) const;
	private:
		struct GenericImportEvent
		{
			static const EventID ID{ HashEventID("GenericImport") };
			struct Args : public EventArgs
			{
				Args(AssetImportSettings&& _importSettings) : importSettings{ std::move(_importSettings) } {}

				AssetImportSettings importSettings;
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
	RuntimeResourceManager::Impl::Impl() = default;
	void RuntimeResourceManager::Impl::Init(const std::filesystem::path& dataPath)
	{
		m_DataPath = dataPath;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error("Failed to initialize TTF: " + std::string(SDL_GetError()));
		}

		m_WorkerThread = std::jthread([this](std::stop_token token) { WorkerLoop(token); });
	}
	AssetHandle RuntimeResourceManager::Impl::AddAsset(Ref<Asset> asset, const std::filesystem::path path)
	{
		std::lock_guard lock(m_Mutex);

		//Check if asset already exists, ifso return its handle
		auto it = m_PathToAssetHandle.find(path);
		if (it != m_PathToAssetHandle.end())
			return it->second;

		AssetHandle handle = asset->GetHandle();
		if (handle != AssetHandle::InvalidUUID)
			throw std::runtime_error("AssetHandle is valid but matching asset cant be found");

		handle = AssetHandle();
		asset->SetHandle(handle);
		m_PathToAssetHandle.emplace(path, handle);
		m_Assets[handle] = asset;
		return handle;
	}
	AssetHandle RuntimeResourceManager::Impl::LoadAsset(const std::filesystem::path& path, AssetType assetType,
		std::unique_ptr<IAssetSpecificImportSettings>&& pSettings,
		AssetLoadMode loadMode)
	{
		auto dataPath = std::filesystem::path(m_DataPath / path);

		//Get the correct importer for the AssetType
		auto importer = AssetImporter::GetImporter(assetType);

		AssetImportSettings importSettings{ path, assetType, std::move(pSettings), loadMode };
		auto virtualPath = importer->GenerateVirtualPath(importSettings);
		importSettings.preGeneratedVirtualPath = virtualPath;

		//Check if the asset already exists
		if (IsAssetLoaded(virtualPath))
			return GetAsset(virtualPath)->GetHandle();

		AssetLoadMode mandatoryLoadMode = importer->GetMandatoryLoadMode();
		if (mandatoryLoadMode != AssetLoadMode::None)
			importSettings.loadMode = mandatoryLoadMode;

		AssetHandle handle{ AssetHandle::InvalidUUID };
		switch (importSettings.loadMode)
		{
		case AssetLoadMode::None:
		case AssetLoadMode::Immediate:
		{
			Ref<Asset> asset = importer->Import(importSettings);
			assert(asset->GetHandle() == AssetHandle::InvalidUUID && "Importer assigned a handle — it shouldn't!");
			handle = AddAsset(asset, virtualPath);
			break;
		}
		case AssetLoadMode::Async:
		{
			importSettings.preGeneratedHandle = AssetHandle();
			{
				std::scoped_lock lock(m_Mutex);
				m_PathToAssetHandle[virtualPath] = importSettings.preGeneratedHandle;
			}
			//will do the import, SetHandle() and AddAsset() on worker thread
			EnqueueLoadEvent(CreateEvent<GenericImportEvent>(std::move(importSettings)));
			break;
		}
		}
		return handle;
	}
	bool RuntimeResourceManager::Impl::IsAssetLoaded(AssetHandle handle) const
	{
		std::scoped_lock lock(m_Mutex);
		return m_Assets.find(handle) != m_Assets.end();
	}
	bool RuntimeResourceManager::Impl::IsAssetLoaded(const std::filesystem::path& path) const
	{
		std::scoped_lock lock(m_Mutex);
		return m_PathToAssetHandle.find(path) != m_PathToAssetHandle.end();
	}
	Ref<Asset> RuntimeResourceManager::Impl::GetAsset(AssetHandle handle) const
	{
		std::scoped_lock lock(m_Mutex);
		auto it = m_Assets.find(handle);
		return it != m_Assets.end() ? it->second : nullptr;
	}
	Ref<Asset> RuntimeResourceManager::Impl::GetAsset(const std::filesystem::path& path) const
	{
		std::scoped_lock lock(m_Mutex);
		auto it = m_PathToAssetHandle.find(path);
		return it != m_PathToAssetHandle.end() ? m_Assets.at(it->second) : nullptr;
	}
	void RuntimeResourceManager::Impl::EnqueueLoadEvent(EventInfo&& event)
	{
		std::scoped_lock lock(m_Mutex);
		m_LoadEventsQueue.push(std::move(event));
		m_Condition.notify_one();
	}
	void RuntimeResourceManager::Impl::WorkerLoop(std::stop_token token)
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
				case GenericImportEvent::ID:
				{
					const auto& importSettings = event.GetArgs<GenericImportEvent>().importSettings;

					auto importer = AssetImporter::GetImporter(importSettings.assetType);
					Ref<Asset> asset = importer->Import(importSettings);
					assert(asset->GetHandle() == AssetHandle::InvalidUUID && "Importer assigned a handle — it shouldn't!");
					asset->SetHandle(importSettings.preGeneratedHandle);
					AddAsset(asset, importSettings.preGeneratedVirtualPath);
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

	RuntimeResourceManager::RuntimeResourceManager() : m_pImpl{ std::make_unique<Impl>() } {}
	RuntimeResourceManager::~RuntimeResourceManager() = default;
	void RuntimeResourceManager::Init(const std::filesystem::path& data) { m_pImpl->Init(data); }
	AssetHandle RuntimeResourceManager::AddAsset(Ref<Asset> asset, const std::filesystem::path& path) { return m_pImpl->AddAsset(std::move(asset), path); }
	AssetHandle RuntimeResourceManager::LoadAsset(const std::filesystem::path& path, AssetType assetType,
		std::unique_ptr<IAssetSpecificImportSettings>&& pSettings,
		AssetLoadMode loadMode) { return m_pImpl->LoadAsset(path, assetType, std::move(pSettings), loadMode); }
	bool RuntimeResourceManager::IsAssetLoaded(AssetHandle handle) const { return m_pImpl->IsAssetLoaded(handle); }
	bool RuntimeResourceManager::IsAssetLoaded(const std::filesystem::path& path) const { return m_pImpl->IsAssetLoaded(path); }
	Ref<Asset> RuntimeResourceManager::GetAsset(AssetHandle handle) const { return m_pImpl->GetAsset(handle); }
	Ref<Asset> RuntimeResourceManager::GetAsset(const std::filesystem::path& path) const { return m_pImpl->GetAsset(path); }
}
