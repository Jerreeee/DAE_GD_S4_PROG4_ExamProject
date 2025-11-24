#pragma once
#include <memory>
#include <filesystem>
#include "JREngine/Audio/ISoundMusic.h"

namespace JRE
{
    class SDLSoundMusic final : public ISoundMusic
    {
    public:
        explicit SDLSoundMusic(const std::filesystem::path& path);
        ~SDLSoundMusic();

        SDLSoundMusic(const SDLSoundMusic&) = delete;
        SDLSoundMusic& operator=(const SDLSoundMusic&) = delete;
        SDLSoundMusic(SDLSoundMusic&& other) noexcept;
        SDLSoundMusic& operator=(SDLSoundMusic&& other) noexcept;

        void Play(bool loop = true) override;
        void Stop() override;
        void SetVolume(float volume) override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl{};
    };
}
