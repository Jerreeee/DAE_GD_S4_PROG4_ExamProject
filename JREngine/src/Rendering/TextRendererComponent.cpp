#include <stdexcept>
#include <memory>
#include "Rendering/SDLRenderer.h"
#include "Asset/Sprite.h"
#include "Asset/Texture2D.h"
#include "Asset/ResourceManager.h"
#include "Rendering/TextRendererComponent.h"

namespace JRE
{
	TextRendererComponent::TextRendererComponent(GameObject& gameObject, const std::string& text, const SoftAssetRef<Font>& softFontRef) :
		RendererComponentBase(gameObject),
		m_Text(text),
		m_SoftFontRef(softFontRef)
	{
		m_NeedsUpdate = text != "";
	}
	void TextRendererComponent::Update()
	{
		if (m_NeedsUpdate)
			UpdateTexture();
	}

	void TextRendererComponent::Render() const
	{
		const auto& pos = GetWorldTransform().GetPosition();
		SDLRenderer::GetInstance().RenderSprite(m_Sprite, pos.x, pos.y);
	}

	// This implementation uses the "dirty flag" pattern
	void TextRendererComponent::SetText(const std::string& text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
	const Sprite& TextRendererComponent::GetSprite() const
	{
		if (m_NeedsUpdate)
			UpdateTexture();
		return *m_Sprite;
	}
	void TextRendererComponent::UpdateTexture() const
	{
		if (!m_NeedsUpdate)
			return;

		if (!m_SoftFontRef.IsLoaded())
			m_SoftFontRef.Get();
		auto textureRef = CreateAssetRef<Texture2D>(m_Text, m_SoftFontRef.Get());
		m_Sprite = CreateAssetRef<Sprite>(SoftAssetRef<Texture2D>(textureRef));
		m_NeedsUpdate = false;
	}
}
