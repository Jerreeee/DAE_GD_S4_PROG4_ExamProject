#include <iostream>
#include "JREngine/Asset/AssetRegistry.h"
#include "JREngine/Asset/AssetLoaderRegistry.h"
#include "EditorResourceManager.h"

namespace JRE
{
	void EditorResourceManager::Init()
	{
		m_WorkerThread = std::jthread([this](std::stop_token token) { WorkerLoop(token); });
	}

	bool EditorResourceManager::IsValidAssetHandle(AssetHandle handle) const
	{
		return AssetRegistry::GetInstance().IsValidAssetHandle(handle);
	}

	bool EditorResourceManager::IsAssetLoaded(AssetHandle handle) const
	{
		std::lock_guard<std::mutex> lock(m_LoadedAssetsMutex);
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	AssetRef<Asset> EditorResourceManager::GetAsset(AssetHandle handle, AssetLoadMode loadMode)
	{
		//Check if the asset is already loaded. If handle is invalid asset should be nullptr
		if (auto asset = TryGetLoadedAsset(handle))
			return asset;

		//Check if handle points to a valid asset. If not, we can't do anything so return nullptr.
		if (!AssetRegistry::GetInstance().IsValidAssetHandle(handle))
			return nullptr;

		const AssetMetadata& metadata = AssetRegistry::GetInstance().GetMetadata(handle);

		//If the load mode is unspecified and the asset can be loaded async then load async
		if (loadMode == AssetLoadMode::Unspecified)
			loadMode = metadata.canLoadAsync ? AssetLoadMode::Async : AssetLoadMode::Immediate;

		auto job = StartOrGetAssetJob(handle, metadata, loadMode);

		AssetRef<Asset> asset = nullptr;
		switch (job->status)
		{
		case LoadState::Loaded:
			asset = job->asset;
			break;
		case LoadState::Loading:
			if (loadMode == AssetLoadMode::Immediate)
				asset = WaitForAssetToLoad(job);
			break;
		//case LoadState::Pending:
		//	Asset is being loaded on another thread but we dont need it immediatly so just return nullptr
		}

		return asset;
	}

	AssetHandle EditorResourceManager::AddAsset(AssetRef<Asset> asset)
	{
		std::lock_guard<std::mutex> lock(m_LoadedAssetsMutex);
		AssetHandle handle{};
		std::cout << "Adding asset with UUID: " << uint64_t(handle) << "\n";
		m_LoadedAssets.emplace(handle, asset);
		return handle;
	}

	AssetRef<Asset> EditorResourceManager::TryGetLoadedAsset(AssetHandle handle)
	{
		std::lock_guard lock(m_LoadedAssetsMutex);
		auto it = m_LoadedAssets.find(handle);
		return it != m_LoadedAssets.end() ? it->second : nullptr;
	}

	std::shared_ptr<EditorResourceManager::AssetLoadJob> EditorResourceManager::StartOrGetAssetJob(AssetHandle handle, const AssetMetadata& metadata, AssetLoadMode loadMode)
	{
		std::unique_lock assetLoadingLock(m_AssetLoadingMutex);

		auto it = m_AssetLoadingJobs.find(handle);
		if (it != m_AssetLoadingJobs.end()) //The asset was already registered for loading
		{
			auto& job = it->second;

			//Take over the job if no other thread is loading the asset yet and we need it immediatly
			if (job->status == LoadState::Pending && loadMode == AssetLoadMode::Immediate)
			{
				job->status = LoadState::Loading;
				assetLoadingLock.unlock(); //Unlock so Load isn't blocking

				//Import the asset
				job->asset = AssetLoaderRegistry::GetInstance().Load(handle, metadata);

				//Ensure the asset is fully loaded before any thread can re-acquire any mutex
				//Otherwise, if we would call "assetLoadingLock.unlock()" after "job->condition.notify_all();"
				//then any waiting thread would see "job->status = LoadState::Loaded;" but the asset
				//would not yet be in m_LoadedAssets which is wrong if "loadMode == AssetLoadMode::Immediate"
				{
					assetLoadingLock.lock();
					job->status = LoadState::Loaded;
					job->condition.notify_all();

					std::lock_guard loadedAssetsLock(m_LoadedAssetsMutex);
					m_LoadedAssets.emplace(handle, job->asset);
					m_AssetLoadingJobs.erase(handle);
					assetLoadingLock.unlock();
				} //loadedAssetsLock unlocks
			}
			//else if (job->status == LoadState::Pending)
			//	Asset is already being loaded by another thread
			//else if (job->status == LoadState::Loaded)
			//	Can't ever happen because then we would have found the asset in m_LoadedAssets

			return job;
		}

		//This is the first thread loading this asset so creata a new job for it
		auto job = std::make_shared<AssetLoadJob>();

		switch (loadMode)
		{
		case AssetLoadMode::Immediate:
		{
			//assetLoadingLock still locked
			job->status = LoadState::Loading;
			assetLoadingLock.unlock(); //Unlock so Load isn't blocking

			//Import
			job->asset = AssetLoaderRegistry::GetInstance().Load(handle, metadata);

			//Same as above Ensure the asset is fully loaded before any thread can re-acquire any mutex
			{
				assetLoadingLock.lock();
				job->status = LoadState::Loaded;
				job->condition.notify_all();
				std::lock_guard loadedAssetsLock(m_LoadedAssetsMutex);
				m_LoadedAssets.emplace(handle, job->asset);
			} //loadedAssetsLock unlocks
			break;
		}
		case AssetLoadMode::Async:
		{
			//assetLoadingLock still locked
			job->status = LoadState::Pending;
			m_AssetLoadingJobs.emplace(handle, job);
			std::lock_guard lock(m_EventQueueMutex);
			m_EventQueue.push(std::move(CreateEvent<GenericImportEvent>(handle, metadata)));
			m_Condition.notify_one();
			break;
		}
		}

		return job;
	} //assetLoadingLock unlocks

	AssetRef<Asset> EditorResourceManager::WaitForAssetToLoad(AssetRef<AssetLoadJob> job)
	{
		std::unique_lock lock(m_AssetLoadingMutex);
		job->condition.wait(lock, [&]() { return job->status == LoadState::Loaded; });
		assert(job->status == LoadState::Loaded && "Job woke up but asset is not loaded!");
		return job->asset;
	}

	void EditorResourceManager::WorkerLoop(std::stop_token token)
	{
		std::unique_lock eventQueueLock(m_EventQueueMutex);

		while (!token.stop_requested())
		{
			m_Condition.wait(eventQueueLock, token, [&] { return !m_EventQueue.empty(); });

			while (!m_EventQueue.empty())
			{
				EventInfo event = std::move(m_EventQueue.front());
				m_EventQueue.pop();
				eventQueueLock.unlock();

				switch (event.GetID())
				{
				case GenericImportEvent::ID:
				{
					const auto& args = event.GetArgs<GenericImportEvent>();

					std::shared_ptr<AssetLoadJob> job = nullptr;

					//Asset was already loaded so we can skip this asset
					{
						std::scoped_lock getAssetLoadJobLock(m_LoadedAssetsMutex, m_AssetLoadingMutex);
						if (m_LoadedAssets.contains(args.handle))
							break;

						auto it = m_AssetLoadingJobs.find(args.handle);
						//job MUST still exist
						assert(it != m_AssetLoadingJobs.end() && "m_AssetLoadingJobs didnt contain the assetLoadJob while it SHOULD");
						job = it->second;
						//LoadState MUST be Pending
						assert(job->status == LoadState::Pending && "LoadState was not Pending while it SHOULD be");
						job->status = LoadState::Loading;
					}

					// Load
					AssetRef<Asset> asset = AssetLoaderRegistry::GetInstance().Load(args.handle, args.metadata);

					//Ensure the asset is considered fully loaded before notifying any waiting threads
					{
						std::scoped_lock  assetLoadedLock(m_LoadedAssetsMutex, m_AssetLoadingMutex);
						m_LoadedAssets.emplace(args.handle, asset);
						m_AssetLoadingJobs.erase(args.handle);
						job->asset = asset;
						job->status = LoadState::Loaded;
						job->condition.notify_all();
					}
					break;
				}
				default:
					assert(false && "Invalid event ID");
					break;
				}
				eventQueueLock.lock();
			}
		}
	}
}
