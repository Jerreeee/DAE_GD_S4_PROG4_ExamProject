#include <stdexcept>
#include <SDL_ttf.h>
#include "TextRendererComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

dae::TextRendererComponent::TextRendererComponent(GameObject& gameObject, const std::string& text, std::shared_ptr<Font> font) :
	ComponentBase(gameObject),
	m_Text(text),
	m_Font(std::move(font)),
	m_TextTexture(nullptr)
{
	m_NeedsUpdate = text != "";
}

void dae::TextRendererComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextRendererComponent::Render() const
{
	if (m_TextTexture)
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextRendererComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextRendererComponent::SetPosition(const float x, const float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}


