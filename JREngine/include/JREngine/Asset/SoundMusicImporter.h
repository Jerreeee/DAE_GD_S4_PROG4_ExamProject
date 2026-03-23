#pragma once
#include "JREngine/Asset/IAssetImporter.h"
#include "JREngine/Audio/ISoundMusic.h"

namespace JRE
{
    class SoundMusicImporter : public IAssetImporter
    {
    public:
        SoundMusicImporter(const std::filesystem::path& filepath);

        static AssetRef<Asset> Load(AssetHandle handle, const AssetMetadata& metadata);

        virtual AssetMetadata GetMetadata() const override;

    private:
        std::filesystem::path m_Path{};
    };
}
