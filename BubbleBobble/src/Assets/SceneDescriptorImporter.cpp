#include <fstream>
#include <sstream>
#include "JREngine/Asset/AssetLoaderRegistry.h"
#include "JREngine/Asset/AssetRegistry.h"
#include "Assets/SceneDescriptor.h"
#include "Assets/SceneDescriptorImporter.h"

using namespace JRE;

namespace BubbleBobble
{
    static bool s_Registered = []()
    {
        AssetLoaderRegistry::GetInstance().RegisterLoader(
            SceneDescriptor::GetStaticType(), SceneDescriptorImporter::Load);
        return true;
    }();

    static std::vector<std::string> Split(const std::string& s, char delim)
    {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string tok;
        while (std::getline(ss, tok, delim))
            out.push_back(tok);
        return out;
    }

    SceneDescriptorImporter::SceneDescriptorImporter(const std::filesystem::path& path)
        : m_Path{ path }
    {
    }

    AssetMetadata SceneDescriptorImporter::GetMetadata() const
    {
        AssetMetadata meta;
        meta.assetType = SceneDescriptor::GetStaticType().data();
        meta.filepath  = m_Path;

        auto fullPath = AssetRegistry::GetInstance().GetFullDatapath(m_Path);
        std::ifstream f(fullPath);
        std::string line;
        while (std::getline(f, line))
        {
            if (line.empty() || line[0] == '/' || line[0] == '#') continue;
            if (line.rfind("entity,", 0) == 0) continue;

            auto comma = line.find(',');
            if (comma == std::string::npos) continue;
            std::string vPath = line.substr(comma + 1);

            AssetHandle dep = AssetRegistry::GetInstance().GetHandleAtPath(vPath);
            if (dep.IsValid())
                meta.dependencies.push_back(dep);
        }
        return meta;
    }

    AssetRef<Asset> SceneDescriptorImporter::Load(AssetHandle, const AssetMetadata& metadata)
    {
        auto desc = CreateAssetRef<SceneDescriptor>();
        auto fullPath = AssetRegistry::GetInstance().GetFullDatapath(metadata.filepath);
        std::ifstream f(fullPath);
        std::string line;

        while (std::getline(f, line))
        {
            if (line.empty() || line[0] == '/' || line[0] == '#') continue;

            auto tokens = Split(line, ',');
            if (tokens.empty()) continue;

            if (tokens[0] == "entity")
            {
                if (tokens.size() < 4) continue;
                EntitySpec spec;
                spec.type = tokens[1];
                spec.x = std::stof(tokens[2]);
                spec.y = std::stof(tokens[3]);
                if (tokens.size() > 4) spec.param = tokens[4];
                desc->entities.push_back(spec);
            }
            else
            {
                if (tokens.size() < 2) continue;
                std::string key   = tokens[0];
                std::string vPath = tokens[1];
                AssetHandle h = AssetRegistry::GetInstance().GetHandleAtPath(vPath);
                desc->assets.emplace(key, h);
            }
        }
        return desc;
    }
}
