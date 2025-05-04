#pragma once
#include <memory>
#include "JREngine/Resources/Asset.h"
#include "JREngine/Math/Rect.h"

namespace JRE
{
	class Texture2D;
	class Sprite final
	{
	public:
		Sprite(AssetHandle textureHandle, const Region& srcRegion);

		Ref<Texture2D> GetTexture() const;
		const Region& GetSrcRegion() const;
	private:
		AssetHandle m_TextureHandle{ AssetHandle::InvalidUUID };
		Ref<Texture2D> m_pTexture{};
		Region m_SrcRegion{};
	};
}