#pragma once
#include "JREngine/Asset/Asset.h"
#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace BubbleBobble
{
    struct EntitySpec
    {
        std::string type;
        float x{}, y{};
        std::string param;
    };

    struct SceneDescriptor : public JRE::Asset
    {
        static constexpr std::string_view GetStaticType() { return "BubbleBobble::SceneDescriptor"; }
        std::string_view GetType() const override { return GetStaticType(); }

        JRE::AssetHandle GetAssetHandle(const std::string& key) const
        {
            auto it = assets.find(key);
            assert(it != assets.end() && "SceneDescriptor: key not found");
            return it->second;
        }

        std::map<std::string, JRE::AssetHandle> assets;
        std::vector<EntitySpec> entities;
    };
}
