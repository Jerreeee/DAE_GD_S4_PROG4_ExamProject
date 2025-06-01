#include <stdexcept>
#include <memory>
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Rendering/SDLRenderer.h"
#include "Asset/ResourceManager.h"
#include "Asset/Font.h"
#include "Asset/Texture2D.h"

#include <iostream>

namespace JRE
{
	Texture2D::Texture2D(const std::filesystem::path& path)
	{
		std::cout << "Creating texture from disk: " << path.string() << "\n";
		m_texture = IMG_LoadTexture(SDLRenderer::GetInstance().GetSDLRenderer(), path.string().c_str());
		if (m_texture == nullptr)
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	Texture2D::Texture2D(const std::string& text, AssetRef<Asset> fontAsset, SDL_Color color)
	{
		if (!fontAsset)
			throw std::runtime_error("Texture2D::Texture2D() | Ref<Asset> fontAsset was nullptr");

		auto font = std::static_pointer_cast<Font>(fontAsset);

		std::cout << "Creating texture from font: " << text << "\n";
		const auto surf = TTF_RenderText_Blended(font->GetFont(), text.c_str(), color);
		if (surf == nullptr)
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		m_texture = SDL_CreateTextureFromSurface(SDLRenderer::GetInstance().GetSDLRenderer(), surf);
		if (m_texture == nullptr)
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		SDL_FreeSurface(surf);
	}

	Texture2D::~Texture2D()
	{
		SDL_DestroyTexture(m_texture);
	}

	SDL_Texture* Texture2D::GetSDLTexture() const
	{
		return m_texture;
	}

	glm::ivec2 Texture2D::GetSize() const
	{
		SDL_Rect dst;
		SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		return { dst.w,dst.h };
	}
}
