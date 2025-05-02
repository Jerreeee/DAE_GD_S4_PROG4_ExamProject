#include "Rendering/SpriteRendererComponent.h"
#include "Core/ServiceLocator.h"
#include "Resources/ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture2D.h"

namespace JRE
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject& gameObject)
		: RendererComponentBase(gameObject)
	{
	}
	void SpriteRendererComponent::Render() const
	{
		if (!m_pTexture) //loading phase, afterwards really cheap
		{
			m_pTexture = ServiceLocator::GetResourceManager().GetTexture(m_TextureHandle);
			if (!m_pTexture) return;
		}

		const auto& pos = GetWorldTransform().GetPosition();
		SDLRenderer::GetInstance().RenderTexture(m_pTexture, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetSprite(ResourceHandle<Texture2D> textureHandle)
	{
		m_TextureHandle = textureHandle;
		m_pTexture.reset();
	}
}
