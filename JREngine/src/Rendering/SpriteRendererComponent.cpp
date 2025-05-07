#include "Rendering/SpriteRendererComponent.h"
#include "Resources/ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Sprite.h"

namespace JRE
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject& gameObject)
		: RendererComponentBase(gameObject)
	{
	}
	void SpriteRendererComponent::Render() const
	{
		if (!m_pSprite) //loading phase, afterwards really cheap
		{
			m_pSprite = ResourceManager::GetAsset<Sprite>(m_SpriteHandle);
			if (!m_pSprite) return;
		}

		const auto& pos = GetWorldTransform().GetPosition();
		SDLRenderer::GetInstance().RenderTexture(m_pSprite, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetSprite(AssetHandle spriteHandle)
	{
		m_SpriteHandle = spriteHandle;
		m_pSprite.reset();
	}
}
