#include <unordered_map>
#include <iostream>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <SDL_mixer.h>

#include "Audio/SDLSoundClip.h"
#include "Audio/SDLSoundSystem.h"

namespace JRE
{
	struct SoundEvent
	{
		Ref<ISoundClip> clip;
		float volume;
	};

	class SDLSoundSystem::Impl
	{
	public:
		Impl();
		~Impl();

		void Play(Ref<ISoundClip> clip, float volume);
	private:
		void SoundThread(std::stop_token token);

		std::queue<SoundEvent> m_SoundQueue{};
		std::mutex m_Mutex{};
		std::condition_variable_any m_Condition{};
		std::jthread m_Worker;
	};
	SDLSoundSystem::Impl::Impl()
		: m_Worker([this](std::stop_token st) { SoundThread(st); })
	{
	}
	SDLSoundSystem::Impl::~Impl() = default;
	void SDLSoundSystem::Impl::Play(Ref<ISoundClip> clip, float volume)
	{
		if (!clip)
		{
			std::cerr << "SDLSoundSystem: Tried to play null SoundClip\n";
			return;
		}

		{
			std::lock_guard lock(m_Mutex);
			m_SoundQueue.push({ clip, volume });
		}
		m_Condition.notify_one();
	}
	void SDLSoundSystem::Impl::SoundThread(std::stop_token token)
	{
		std::unique_lock lock(m_Mutex);

		while (!token.stop_requested())
		{
			m_Condition.wait(lock, token, [&] { return !m_SoundQueue.empty(); });

			while (!m_SoundQueue.empty())
			{
				SoundEvent evt = std::move(m_SoundQueue.front());
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
	void SDLSoundSystem::Play(Ref<ISoundClip> clip, float volume) { m_pImpl->Play(clip, volume); }
}
