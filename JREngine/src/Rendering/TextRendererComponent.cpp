#include <stdexcept>
#include <memory>
#include "Rendering/Renderer.h"
#include "Resources/Font.h"
#include "Rendering/Sprite.h"
#include "Rendering/Texture2D.h"
#include "Resources/ResourceManager.h"
#include "Rendering/TextRendererComponent.h"

namespace JRE
{
	TextRendererComponent::TextRendererComponent(GameObject& gameObject, const std::string& text, AssetHandle fontHandle) :
		RendererComponentBase(gameObject),
		m_Text(text),
		m_FontHandle(fontHandle)
	{
		m_NeedsUpdate = text != "";
	}
	void TextRendererComponent::Update()
	{
		if (m_NeedsUpdate)
		{
			auto texture = CreateRef<Texture2D>(m_Text, m_FontHandle);
			auto textureHandle = ResourceManager::AddAsset(texture);
			m_pSprite = CreateRef<Sprite>(textureHandle);
			m_SpriteHandle = ResourceManager::AddAsset(m_pSprite);
			m_NeedsUpdate = false;
		}
	}

	void TextRendererComponent::Render() const
	{
		if (m_pSprite)
		{
			const auto& pos = GetWorldTransform().GetPosition();
			SDLRenderer::GetInstance().RenderTexture(m_SpriteHandle, pos.x, pos.y);
		}
	}

	// This implementation uses the "dirty flag" pattern
	void TextRendererComponent::SetText(const std::string& text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}
