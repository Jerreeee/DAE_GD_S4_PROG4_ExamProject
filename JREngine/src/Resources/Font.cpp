#include <stdexcept>
#include <SDL_ttf.h>
#include "Resources/Font.h"

namespace JRE
{
	Font::Font(const std::filesystem::path& path, unsigned int size)
	{
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
