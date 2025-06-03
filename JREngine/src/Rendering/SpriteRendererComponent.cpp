#include "Asset/ResourceManager.h"
#include "Rendering/SDLRenderer.h"
#include "Asset/Sprite.h"
#include "Rendering/SpriteRendererComponent.h"

namespace JRE
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject& gameObject)
		: RendererComponentBase(gameObject)
	{
	}
	void SpriteRendererComponent::Render() const
	{
		const auto& pos = GetWorldTransform().GetPosition();
		SDLRenderer::GetInstance().RenderSprite(m_Sprite, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetSprite(const AssetRef<Sprite>& sprite)
	{
		m_Sprite = sprite;
	}
}
