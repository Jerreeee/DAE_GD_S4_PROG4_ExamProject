#include <unordered_map>
#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <SDL_mixer.h>

#include "Audio/SDLSoundClip.h"
#include "Audio/SDLSoundMusic.h"
#include "Audio/SDLSoundSystem.h"
#include "Asset/AssetMetadata.h"
#include "Asset/AssetRegistry.h"

namespace JRE
{
	struct PlaySoundClipEvent
	{
		AssetRef<ISoundClip> clip;
		float volume;
	};

	class SDLSoundSystem::Impl
	{
	public:
		Impl();
		~Impl();

		AssetRef<ISoundClip> CreateSoundClip(AssetHandle, const AssetMetadata& metadata);
		void Play(AssetRef<ISoundClip> soundClipAsset, float volume);

		AssetRef<ISoundMusic> CreateMusic(AssetHandle, const AssetMetadata&);
		void PlayMusic(AssetRef<ISoundMusic> music, bool loop, float volume);
		void StopMusic();
	private:
		void SoundThread(std::stop_token token);

		std::queue<PlaySoundClipEvent> m_SoundQueue{};
		std::mutex m_Mutex{};
		std::condition_variable_any m_Condition{};
		std::jthread m_Worker;
	};
	SDLSoundSystem::Impl::Impl()
		: m_Worker([this](std::stop_token st) { SoundThread(st); })
	{
	}
	SDLSoundSystem::Impl::~Impl() = default;
	AssetRef<ISoundClip> SDLSoundSystem::Impl::CreateSoundClip(AssetHandle, const AssetMetadata& metadata)
	{
		auto fullPath = AssetRegistry::GetInstance().GetFullDatapath(metadata.filepath);
		return CreateAssetRef<SDLSoundClip>(fullPath);
	}
	void SDLSoundSystem::Impl::Play(AssetRef<ISoundClip> soundClipAsset, float volume)
	{
		if (!soundClipAsset)
		{
			std::cerr << "SLDSoundSystem::Play() | Ref<ISoundClip> clip was nullptr\n";
			return;
		}

		auto soundClip = std::static_pointer_cast<SDLSoundClip>(soundClipAsset);

		{
			std::lock_guard lock(m_Mutex);
			m_SoundQueue.emplace(PlaySoundClipEvent{ soundClip, volume });
		}
		m_Condition.notify_one();
	}

	AssetRef<ISoundMusic> SDLSoundSystem::Impl::CreateMusic(AssetHandle, const AssetMetadata& metadata)
	{
		auto fullPath = AssetRegistry::GetInstance().GetFullDatapath(metadata.filepath);
		return CreateAssetRef<SDLSoundMusic>(fullPath);
	}

	void SDLSoundSystem::Impl::PlayMusic(AssetRef<ISoundMusic> music, bool loop, float volume)
	{
		if (!music) return;
		music->SetVolume(volume);
		music->Play(loop);
	}

	void SDLSoundSystem::Impl::StopMusic()
	{
		Mix_HaltMusic();
	}

	void SDLSoundSystem::Impl::SoundThread(std::stop_token token)
	{
		std::unique_lock lock(m_Mutex);

		while (!token.stop_requested())
		{
			m_Condition.wait(lock, token, [&] { return !m_SoundQueue.empty(); });

			while (!m_SoundQueue.empty())
			{
				PlaySoundClipEvent evt = std::move(m_SoundQueue.front());
				m_SoundQueue.pop();

				lock.unlock();
				if (evt.clip)
				{
					evt.clip->SetVolume(std::clamp(evt.volume, 0.0f, 1.0f));
					evt.clip->Play();
				}
				lock.lock();
			}
		}
	}

	SDLSoundSystem::SDLSoundSystem() : m_pImpl{ std::make_unique<Impl>() } {}
	SDLSoundSystem::~SDLSoundSystem() = default;
	AssetRef<ISoundClip> SDLSoundSystem::CreateSoundClip(AssetHandle handle, const AssetMetadata& metadata) { return m_pImpl->CreateSoundClip(handle, metadata); }
	void SDLSoundSystem::Play(AssetRef<ISoundClip> soundClipAsset, float volume) { m_pImpl->Play(soundClipAsset, volume); }
	AssetRef<ISoundMusic> SDLSoundSystem::CreateMusic(AssetHandle h, const AssetMetadata& m) { return m_pImpl->CreateMusic(h, m); }
	void SDLSoundSystem::PlayMusic(AssetRef<ISoundMusic> music, bool loop, float volume) { m_pImpl->PlayMusic(music, loop, volume); }
	void SDLSoundSystem::StopMusic() { m_pImpl->StopMusic(); }
}
