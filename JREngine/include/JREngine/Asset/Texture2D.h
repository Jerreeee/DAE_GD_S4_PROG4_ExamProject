#pragma once
#include <vec2.hpp>
#include <string>
#include <filesystem>
#include "JREngine/Asset/Asset.h"

struct SDL_Texture;
namespace JRE
{
	class Font;

	//Simple RAII wrapper for an SDL_Texture
	class Texture2D final : public Asset
	{
	public:
		explicit Texture2D(const std::string& text, AssetHandle fontHandle);
		explicit Texture2D(const std::filesystem::path& path);
		~Texture2D();
		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;
		glm::ivec2 GetSize() const;

		static AssetType GetStaticType() { return AssetType::Texture2D; };
		virtual AssetType GetType() const override { return GetStaticType(); };
	private:
		SDL_Texture* m_texture{};
	};
}
