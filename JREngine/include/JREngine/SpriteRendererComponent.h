#pragma once
#include <string>
#include "ResourceHandle.h"
#include "RendererComponentBase.h"

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

		void SetTexture(const std::string& filename);
	private:
		ResourceHandle<Texture2D> m_TextureHandle{};
		mutable std::shared_ptr<Texture2D> m_pTexture{};
	};
}

