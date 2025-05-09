#include "Rendering/SpriteRendererComponent.h"
#include "Asset/ResourceManager.h"
#include "Rendering/SDLRenderer.h"
#include "Asset/Sprite.h"

namespace JRE
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject& gameObject)
		: RendererComponentBase(gameObject)
	{
	}
	void SpriteRendererComponent::Render() const
	{
		if (!m_Sprite)
			m_Sprite = ResourceManager::TryGetAsset<Sprite>(m_SpriteHandle);

		const auto& pos = GetWorldTransform().GetPosition();
		SDLRenderer::GetInstance().RenderTexture(m_Sprite, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetSprite(AssetHandle spriteHandle)
	{
		m_SpriteHandle = spriteHandle;
	}
}
