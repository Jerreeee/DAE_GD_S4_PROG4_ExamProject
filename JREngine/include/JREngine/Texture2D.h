#pragma once
#include <vec2.hpp>
#include <string>
#include "ResourceHandle.h"

struct SDL_Texture;
namespace JRE
{
	class Font;
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(const std::string& fullPath);
		explicit Texture2D(const std::string& fullPath, ResourceHandle<Font> fontHandle);
		~Texture2D();

		glm::ivec2 GetSize() const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_texture{};
	};
}
