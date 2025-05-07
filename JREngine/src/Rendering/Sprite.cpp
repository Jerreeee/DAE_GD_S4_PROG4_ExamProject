#include <stdexcept>
#include <sstream>
#include "Resources/ResourceManager.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Sprite.h"

namespace JRE
{
    std::filesystem::path Sprite::PathBuilder::Build(AssetHandle textureHandle, const Region& srcRegion)
    {
        std::stringstream path{};
        path << "Sprite@" << static_cast<uint64_t>(textureHandle)
            << "@" << srcRegion.x << "@" << srcRegion.y << "@" << srcRegion.width << "@" << srcRegion.height;
        return std::filesystem::path(path.str());
    }

    std::filesystem::path Sprite::PathBuilder::Build(AssetHandle textureHandle)
    {
        std::stringstream path{};
        path << "Sprite/@" << static_cast<uint64_t>(textureHandle);
        return std::filesystem::path(path.str());
    }

    Sprite::Sprite(AssetHandle textureHandle, const Region& srcRegion) :
        m_TextureHandle{ textureHandle },
        m_SrcRegion{ srcRegion }
    {
    }

    Sprite::Sprite(AssetHandle textureHandle) :
        m_TextureHandle{ textureHandle }
    {
        Ref<Texture2D> tex = ResourceManager::GetAsset<Texture2D>(textureHandle);
        if (!tex)
            throw std::runtime_error("No texture found");

        glm::ivec2 size = tex->GetSize();
        m_SrcRegion = Region{ 0, 0, size.x, size.y };
    }

    Ref<Texture2D> Sprite::GetTexture() const
    {
        return m_pTexture;
    }

    const Region& Sprite::GetSrcRegion() const
    {
        return m_SrcRegion;
    }
}