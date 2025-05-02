#pragma once
#include <memory>
#include "ResourceHandle.h"

namespace JRE
{
	class Texture2D;
	class Sprite final
	{
	public:
		//Sprite(ResourceHandle<Texture2D> textureHandle, int x);
	private:
		ResourceHandle<Texture2D> m_TextureHandle{};
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}