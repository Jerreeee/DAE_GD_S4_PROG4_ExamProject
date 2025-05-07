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
		if (!m_pFont) //loading phase, afterwards really cheap
		{
			m_pFont = ResourceManager::GetAsset<Font>(m_FontHandle);
			if (!m_pFont) return;
		}

		if (m_NeedsUpdate)
		{
			auto textureHandle = ResourceManager::CreateAsset<Texture2D>(m_Text, m_FontHandle);
			if (textureHandle == AssetHandle::InvalidUUID)
				throw std::runtime_error("error");
			m_SpriteHandle = ResourceManager::CreateAsset<Sprite>(textureHandle);
			m_pSprite = ResourceManager::GetAsset<Sprite>(m_SpriteHandle);
			m_NeedsUpdate = false;
		}
	}

	void TextRendererComponent::Render() const
	{
		if (m_pSprite)
		{
			const auto& pos = GetWorldTransform().GetPosition();
			SDLRenderer::GetInstance().RenderTexture(m_pSprite, pos.x, pos.y);
		}
	}

	// This implementation uses the "dirty flag" pattern
	void TextRendererComponent::SetText(const std::string& text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

