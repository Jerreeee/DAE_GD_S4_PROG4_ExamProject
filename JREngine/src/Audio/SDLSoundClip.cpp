#include <SDL_mixer.h>
#include <stdexcept>
#include <mutex>
#include "Audio/SDLSoundClip.h"

#include <iostream>

struct Mix_Chunk;

namespace JRE
{
	class SDLSoundClip::Impl final
	{
	public:
		explicit Impl(const std::filesystem::path& path);
		~Impl();

		Impl(const Impl&) = delete;
		Impl& operator=(const Impl&) = delete;
		Impl(Impl&& other) noexcept;
		Impl& operator=(Impl&& other) noexcept;

		void Play(int loops, int channel) const;
		void SetVolume(float volume);

	private:
		Mix_Chunk* m_pChunk{};
	};
	SDLSoundClip::Impl::Impl(const std::filesystem::path& path)
	{
		std::cout << "Creating SDLSoundClip: " << path.string() << "\n";

		//Mix_LoadWAV is not threadsafe, therefore the mutex
		static std::mutex mutex{};
		std::lock_guard<std::mutex> lock(mutex);
		m_pChunk = Mix_LoadWAV(path.string().c_str());
		if (!m_pChunk)
			throw std::runtime_error("Failed to load WAV file: " + path.string() + " | SDL_mixer Error: " + Mix_GetError());
	}
	SDLSoundClip::Impl::~Impl()
	{
		if (m_pChunk)
			Mix_FreeChunk(m_pChunk);
	}
	SDLSoundClip::Impl::Impl(Impl&& other) noexcept :
		m_pChunk{other.m_pChunk }
	{
		other.m_pChunk = nullptr;
	}
	SDLSoundClip::Impl& SDLSoundClip::Impl::operator=(Impl&& other) noexcept
	{
		if (this != &other)
		{
			if (m_pChunk) Mix_FreeChunk(m_pChunk);
			m_pChunk = other.m_pChunk;
			other.m_pChunk = nullptr;
		}
		return *this;
	}

	void SDLSoundClip::Impl::Play(int loops, int channel) const { Mix_PlayChannel(channel, m_pChunk, loops); }
	void SDLSoundClip::Impl::SetVolume(float volume) { Mix_VolumeChunk(m_pChunk, static_cast<int>(volume * MIX_MAX_VOLUME)); }
	SDLSoundClip::SDLSoundClip(const std::filesystem::path& path) : m_pImpl{ std::make_unique<Impl>(path) } {}
	SDLSoundClip::~SDLSoundClip() = default;
	SDLSoundClip::SDLSoundClip(SDLSoundClip&& other) noexcept : m_pImpl{ std::move(other.m_pImpl) } {}
	SDLSoundClip& SDLSoundClip::operator=(SDLSoundClip&& other) noexcept { if (this != &other) m_pImpl = std::move(other.m_pImpl); return *this; }
	void SDLSoundClip::Play(int loops, int channel) const { m_pImpl->Play(loops, channel); }
	void SDLSoundClip::SetVolume(float volume) { m_pImpl->SetVolume(volume); }
}
