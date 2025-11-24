#include <SDL_mixer.h>
#include <stdexcept>
#include <mutex>
#include <iostream>
#include "Audio/SDLSoundMusic.h"

namespace JRE
{
    class SDLSoundMusic::Impl
    {
    public:
        explicit Impl(const std::filesystem::path& path);
        ~Impl();

        Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;
        Impl(Impl&& other) noexcept;
        Impl& operator=(Impl&& other) noexcept;

        void Play(bool loop);
        void Stop();
        void SetVolume(float volume);

    private:
        Mix_Music* m_pMusic{};
    };

    SDLSoundMusic::Impl::Impl(const std::filesystem::path& path)
    {
        std::cout << "Creating SDLSoundMusic: " << path.string() << "\n";

        // SDL_mixer loading is NOT threadsafe. Use global mutex.
        static std::mutex mutex{};
        std::lock_guard<std::mutex> lock(mutex);

        m_pMusic = Mix_LoadMUS(path.string().c_str());
        if (!m_pMusic)
            throw std::runtime_error("Failed to load Music file: " + path.string() + " | SDL_mixer Error: " + Mix_GetError());
    }

    SDLSoundMusic::Impl::~Impl()
    {
        if (m_pMusic)
            Mix_FreeMusic(m_pMusic);
    }

    SDLSoundMusic::Impl::Impl(Impl&& other) noexcept
        : m_pMusic{ other.m_pMusic }
    {
        other.m_pMusic = nullptr;
    }

    SDLSoundMusic::Impl& SDLSoundMusic::Impl::operator=(Impl&& other) noexcept
    {
        if (this != &other)
        {
            if (m_pMusic)
                Mix_FreeMusic(m_pMusic);

            m_pMusic = other.m_pMusic;
            other.m_pMusic = nullptr;
        }
        return *this;
    }

    void SDLSoundMusic::Impl::Play(bool loop)
    {
        if (!m_pMusic)
            return;

        int loops = loop ? -1 : 1;
        Mix_PlayMusic(m_pMusic, loops);
    }

    void SDLSoundMusic::Impl::Stop()
    {
        Mix_HaltMusic();
    }

    void SDLSoundMusic::Impl::SetVolume(float volume)
    {
        Mix_VolumeMusic(int(volume * MIX_MAX_VOLUME));
    }

    SDLSoundMusic::SDLSoundMusic(const std::filesystem::path& path) : m_pImpl(std::make_unique<Impl>(path)) {}
    SDLSoundMusic::~SDLSoundMusic() = default;
    SDLSoundMusic::SDLSoundMusic(SDLSoundMusic&& other) noexcept : m_pImpl(std::move(other.m_pImpl)) {}
    SDLSoundMusic& SDLSoundMusic::operator=(SDLSoundMusic&& other) noexcept { if (this != &other) m_pImpl = std::move(other.m_pImpl); return *this; }
    void SDLSoundMusic::Play(bool loop) { m_pImpl->Play(loop); }
    void SDLSoundMusic::Stop() { m_pImpl->Stop(); }
    void SDLSoundMusic::SetVolume(float volume) { m_pImpl->SetVolume(volume); }
}
