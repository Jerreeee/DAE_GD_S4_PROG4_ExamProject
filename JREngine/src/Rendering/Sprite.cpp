#include "Rendering/Sprite.h"

namespace JRE
{
    Sprite::Sprite(AssetHandle textureHandle, const Region& srcRegion) :
        m_TextureHandle{ textureHandle },
        m_SrcRegion{ srcRegion }
    {
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