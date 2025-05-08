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
		const auto& pos = GetWorldTransform().GetPosition();
		SDLRenderer::GetInstance().RenderTexture(m_SpriteHandle, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetSprite(AssetHandle spriteHandle)
	{
		m_SpriteHandle = spriteHandle;
	}
}
