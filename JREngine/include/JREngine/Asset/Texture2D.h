#pragma once
#include "SDL.h"
#include <vec2.hpp>
#include <string>
#include <filesystem>
#include "JREngine/Asset/Asset.h"

struct SDL_Texture;
struct SDL_Color;
namespace JRE
{
	class Font;

	//Simple RAII wrapper for an SDL_Texture
	class Texture2D final : public Asset
	{
	public:
		REGISTER_TYPE_HEADER(Texture2D, AssetTypeInfo, "JRE::Texture2D");

		explicit Texture2D(const std::string& text, AssetRef<Asset> fontAsset, SDL_Color color);
		explicit Texture2D(const std::filesystem::path& path);
		~Texture2D();
		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;
		glm::ivec2 GetSize() const;
	private:
		SDL_Texture* m_texture{};
	};
}
