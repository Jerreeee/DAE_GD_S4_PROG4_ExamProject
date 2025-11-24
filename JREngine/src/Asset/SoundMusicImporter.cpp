#include "Core/ServiceLocator.h"
#include "Audio/ISoundMusic.h"
#include "Audio/ISoundSystem.h"
#include "Asset/AssetImporter.h"
#include "Asset/SoundMusicImporter.h"

namespace JRE
{
    // Register importer automatically at startup
    static bool s_RegisteredMusic = []()
        {
            AssetImporter::GetInstance().RegisterImporter(
                ISoundMusic::GetStaticType(),
                SoundMusicImporter::ImportAsset
            );
            return true;
        }();

    SoundMusicImporter::SoundMusicImporter(const std::filesystem::path& filepath)
        : m_Path{ AssetImporter::GetInstance().GetFullDatapath(filepath) }
    {
    }

    AssetRef<Asset> SoundMusicImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
    {
        return ServiceLocator::GetSoundSystem().CreateMusic(handle, metadata);
    }

    AssetMetadata SoundMusicImporter::GetMetadata() const
    {
        return AssetMetadata{ISoundMusic::GetStaticType().data(), m_Path, "", true};
    }
}
