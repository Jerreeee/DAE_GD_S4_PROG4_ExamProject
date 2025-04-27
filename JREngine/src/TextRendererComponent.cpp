#include <stdexcept>
#include <memory>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ServiceLocator.h"
#include "IResourceManager.h"
#include "TextRendererComponent.h"

JRE::TextRendererComponent::TextRendererComponent(GameObject& gameObject, const std::string& text, ResourceHandle<Font> fontHandle) :
	RendererComponentBase(gameObject),
	m_Text(text),
	m_FontHandle(fontHandle)
{
	if (!fontHandle.IsValid())
		throw std::runtime_error(std::string("Not a valid font"));

	m_NeedsUpdate = text != "";
}
void JRE::TextRendererComponent::Update()
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

void JRE::TextRendererComponent::Render() const
{
	if (m_pTextTexture)
	{
		const auto& pos = GetWorldTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(m_pTextTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void JRE::TextRendererComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
