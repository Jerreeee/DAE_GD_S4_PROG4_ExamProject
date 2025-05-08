#include <stdexcept>
#include <sstream>
#include "Resources/ResourceManager.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Sprite.h"

namespace JRE
{
    Sprite::Sprite(AssetHandle textureHandle, const Region& srcRegion) :
        m_TextureHandle{ textureHandle },
        m_UsesWholeTextureRegion{ false },
        m_SrcRegion{ srcRegion }
    {
    }

    Sprite::Sprite(AssetHandle textureHandle) :
        m_TextureHandle{ textureHandle },
        m_UsesWholeTextureRegion{ true }
    {
    }

    Ref<Texture2D> Sprite::GetTexture() const
    {
        if (!m_pTexture)
            ForceLoad();
        return m_pTexture;
    }

    const Region& Sprite::GetSrcRegion() const
    {
        if (!m_pTexture)
            ForceLoad();
        return m_SrcRegion;
    }
    void Sprite::ForceLoad() const
    {
        m_pTexture = ResourceManager::GetAsset<Texture2D>(m_TextureHandle);
        if (m_UsesWholeTextureRegion)
        {
            glm::ivec2 size = m_pTexture->GetSize();
            m_SrcRegion = Region{ 0, 0, size.x, size.y };
        }
    }
}
