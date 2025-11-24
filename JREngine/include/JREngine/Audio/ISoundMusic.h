#pragma once
#include "JREngine/Asset/Asset.h"

namespace JRE
{
    class ISoundMusic : public Asset
    {
    public:
        virtual void Play(bool loop) = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(float percentage) = 0;

        static constexpr std::string_view GetStaticType() { return "SoundMusic"; };
        virtual std::string_view GetType() const override { return GetStaticType(); };
    };

    class NullSoundMusic final : public ISoundMusic
    {
    public:
        void Play(bool) override {};
        void Stop() override {};
        void SetVolume(float) override {};
    };
}
