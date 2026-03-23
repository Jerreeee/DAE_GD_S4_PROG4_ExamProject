#pragma once
#include <filesystem>
#include "JREngine/Asset/IAssetImporter.h"

namespace BubbleBobble
{
    class SceneDescriptorImporter final : public JRE::IAssetImporter
    {
    public:
        explicit SceneDescriptorImporter(const std::filesystem::path& path);

        JRE::AssetMetadata GetMetadata() const override;
        static JRE::AssetRef<JRE::Asset> ImportAsset(JRE::AssetHandle handle,
                                                      const JRE::AssetMetadata& metadata);
    private:
        std::filesystem::path m_Path;
    };
}
