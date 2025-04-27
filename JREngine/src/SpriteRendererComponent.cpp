#include "SpriteRendererComponent.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "Renderer.h"

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
		Renderer::GetInstance().RenderTexture(m_pTexture, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetTexture(const std::string& filename)
	{
		m_TextureHandle = ServiceLocator::GetResourceManager().LoadTexture(filename);
	}
}
