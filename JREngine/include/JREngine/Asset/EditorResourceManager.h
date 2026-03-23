#pragma once
#include <filesystem>
#include <functional>
#include <string>
#include <memory>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <stop_token>

#include "JREngine/Core/Event.h"
#include "JREngine/Asset/IResourceManager.h"

namespace JRE
{
	class EditorResourceManager final : public IResourceManager
	{
	public:
		void Init();
		void SerializeManifest(const std::filesystem::path& manifestPath) const;

		virtual bool IsValidAssetHandle(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		virtual AssetRef<Asset> GetAsset(AssetHandle handle, AssetLoadMode loadMode = AssetLoadMode::Unspecified) override;
		virtual AssetHandle AddAsset(AssetRef<Asset> asset) override;
	private:
		struct GenericImportEvent
		{
			static const EventID ID{ HashEventID("GenericImport") };
			struct Args : public EventArgs
			{
				Args(AssetHandle _handle, const AssetMetadata& _metadata) :
					handle{ _handle },
					metadata{ _metadata }
				{
				}

				AssetHandle handle;
				AssetMetadata metadata;
			};
		};

		enum class LoadState
		{
			Pending, Loading, Loaded
		};

		struct AssetLoadJob
		{
			LoadState status = LoadState::Pending;
			AssetRef<Asset> asset = nullptr;
			std::condition_variable_any condition;
		};

		AssetRef<Asset> TryGetLoadedAsset(AssetHandle handle);
		std::shared_ptr<AssetLoadJob> StartOrGetAssetJob(AssetHandle handle, const AssetMetadata& metadata, AssetLoadMode loadMode);
		AssetRef<Asset> WaitForAssetToLoad(AssetRef<AssetLoadJob> job);
		void WorkerLoop(std::stop_token token);

		mutable std::mutex m_LoadedAssetsMutex{};
		std::map<AssetHandle, AssetRef<Asset>> m_LoadedAssets{};

		mutable std::mutex m_AssetLoadingMutex{};
		std::unordered_map<AssetHandle, std::shared_ptr<AssetLoadJob>> m_AssetLoadingJobs{};

		mutable std::mutex m_EventQueueMutex{};
		std::queue<EventInfo> m_EventQueue{};
		std::condition_variable_any m_Condition{};
		std::jthread m_WorkerThread{};
	};
}
