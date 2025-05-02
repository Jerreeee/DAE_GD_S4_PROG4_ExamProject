#include <stdexcept>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "Font.h"
#include "Texture2D.h"

JRE::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 JRE::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* JRE::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

JRE::Texture2D::Texture2D(const std::string &fullPath)
{
	m_texture = IMG_LoadTexture(SDLRenderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (m_texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
}

JRE::Texture2D::Texture2D(const std::string& text, ResourceHandle<Font> fontHandle)
{
	auto pFont = ServiceLocator::GetResourceManager().GetFont(fontHandle);
	if (!pFont) return;

	const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
	const auto surf = TTF_RenderText_Blended(pFont->GetFont(), text.c_str(), color);
	if (surf == nullptr)
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

	m_texture = SDL_CreateTextureFromSurface(SDLRenderer::GetInstance().GetSDLRenderer(), surf);
	if (m_texture == nullptr)
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	SDL_FreeSurface(surf);
}
