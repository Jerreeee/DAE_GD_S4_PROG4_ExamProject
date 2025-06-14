#include <stdexcept>
#include <sstream>
#include "Asset/ResourceManager.h"
#include "Asset/Sprite.h"

#include <iostream>

namespace JRE
{
    Sprite::Sprite(const SoftAssetRef<Texture2D>& softTextureRef, const Region& srcRegion) :
        m_SoftTextureRef{ softTextureRef },
        m_UsesWholeTextureRegion{ false },
        m_SrcRegion{ srcRegion }
    {
        //std::cout << "Creating sprite using texturehandle and region\n";
    }

    Sprite::Sprite(const SoftAssetRef<Texture2D>& softTextureRef) :
        m_SoftTextureRef{ softTextureRef },
        m_UsesWholeTextureRegion{ true }
    {
        //std::cout << "Creating sprite using texturehandle\n";
    }

    AssetRef<Texture2D> Sprite::GetTexture() const
    {
        if (!m_IsInitialized)
            Initialize();
        return m_SoftTextureRef.Get();
    }

    const Region& Sprite::GetSrcRegion() const
    {
        if (!m_IsInitialized)
            Initialize();
        return m_SrcRegion;
    }
    void Sprite::Initialize() const
    {
        m_SoftTextureRef.Get();
        if (m_UsesWholeTextureRegion)
        {
            glm::ivec2 size = m_SoftTextureRef.Get()->GetSize();
            m_SrcRegion = Region{ 0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y) };
        }
        m_IsInitialized = true;
    }
}
