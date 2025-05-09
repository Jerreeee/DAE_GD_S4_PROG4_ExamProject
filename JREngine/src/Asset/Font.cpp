#include <stdexcept>
#include <SDL_ttf.h>
#include "Asset/Font.h"

#include <iostream>

namespace JRE
{
	Font::Font(const std::filesystem::path& path, unsigned int size)
	{
		std::cout << "Creating font with size: " << std::to_string(size) << "\n";
		m_pFont = TTF_OpenFont(path.string().c_str(), size);
		if (m_pFont == nullptr)
			throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}

	Font::~Font()
	{
		TTF_CloseFont(m_pFont);
	}

	TTF_Font* Font::GetFont() const {
		return m_pFont;
	}
}
