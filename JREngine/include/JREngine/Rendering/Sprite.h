#pragma once
#include <memory>
#include "JREngine/Resources/ResourceHandle.h"
#include "JREngine/Math/Rect.h"

namespace JRE
{
	class Texture2D;
	class Sprite final
	{
	public:
		Sprite(ResourceHandle<Texture2D> textureHandle, const Region& srcRegion);

		std::shared_ptr<Texture2D> GetTexture() const;
		const Region& GetSrcRegion() const;
	private:
		ResourceHandle<Texture2D> m_TextureHandle{};
		std::shared_ptr<Texture2D> m_pTexture{};
		Region m_SrcRegion{};
	};
}