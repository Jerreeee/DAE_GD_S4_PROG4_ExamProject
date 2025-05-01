#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include "Event.h"
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
		virtual ResourceHandle<ISoundClip> LoadSound(const std::string& file) override;

		virtual std::shared_ptr<Texture2D> GetTexture(ResourceHandle<Texture2D> handle) const override;
		virtual std::shared_ptr<Font> GetFont(ResourceHandle<Font> handle) const override;
		virtual std::shared_ptr<ISoundClip> GetSound(ResourceHandle<ISoundClip> handle) const override;
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

		std::filesystem::path m_dataPath{};

		void UnloadUnusedResources();
		
		//Not ideal yet because GUID is just an incrementing int
		//so idx 0-2 might be a texture, 3 a font and 4 a Sound
		//this means each map will have a lot of missing indices
		//but its a start :)
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
}
