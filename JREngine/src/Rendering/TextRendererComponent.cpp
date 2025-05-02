#include <stdexcept>
#include <memory>
#include "Rendering/Renderer.h"
#include "Resources/Font.h"
#include "Rendering/Texture2D.h"
#include "Core/ServiceLocator.h"
#include "Resources/IResourceManager.h"
#include "Rendering/TextRendererComponent.h"

namespace JRE
{
	TextRendererComponent::TextRendererComponent(GameObject& gameObject, const std::string& text, ResourceHandle<Font> fontHandle) :
		RendererComponentBase(gameObject),
		m_Text(text),
		m_FontHandle(fontHandle)
	{
		if (!fontHandle.IsValid())
			throw std::runtime_error(std::string("Not a valid font"));

		m_NeedsUpdate = text != "";
	}
	void TextRendererComponent::Update()
	{
		if (!m_pFont) //loading phase, afterwards really cheap
		{
			m_pFont = ServiceLocator::GetResourceManager().GetFont(m_FontHandle);
			if (!m_pFont) return;
		}

		if (m_NeedsUpdate)
		{
			m_TextTextureHandle = ServiceLocator::GetResourceManager().LoadTexture(m_Text, m_FontHandle);
			m_pTextTexture = ServiceLocator::GetResourceManager().GetTexture(m_TextTextureHandle);
			m_NeedsUpdate = false;
		}
	}

	void TextRendererComponent::Render() const
	{
		if (m_pTextTexture)
		{
			const auto& pos = GetWorldTransform().GetPosition();
			SDLRenderer::GetInstance().RenderTexture(m_pTextTexture, pos.x, pos.y);
		}
	}

	// This implementation uses the "dirty flag" pattern
	void TextRendererComponent::SetText(const std::string& text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

