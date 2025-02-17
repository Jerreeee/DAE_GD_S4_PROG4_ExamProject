#include "SpriteRendererComponent.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "Renderer.h"

namespace dae
{
	SpriteRendererComponent::SpriteRendererComponent(GameObject& gameObject)
		: ComponentBase(gameObject)
	{
	}
	void SpriteRendererComponent::Render() const
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
	void SpriteRendererComponent::SetTexture(const std::string& filename)
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	}
}
