#pragma once
#include <string>
#include "JREngine/Resources/Asset.h"
#include "JREngine/Rendering/RendererComponentBase.h"

namespace JRE
{
	class Texture2D;
	class SpriteRendererComponent final: public RendererComponentBase
	{
	public:
		SpriteRendererComponent(GameObject& gameObject);

		SpriteRendererComponent(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent(SpriteRendererComponent&& other) = delete;
		SpriteRendererComponent& operator=(const SpriteRendererComponent& other) = delete;
		SpriteRendererComponent& operator=(SpriteRendererComponent&& other) = delete;

		virtual void Render() const override;

		void SetSprite(AssetHandle textureHandle);
	private:
		AssetHandle m_TextureHandle{ AssetHandle::InvalidUUID };
		mutable Ref<Texture2D> m_pTexture{ nullptr };
	};
}

