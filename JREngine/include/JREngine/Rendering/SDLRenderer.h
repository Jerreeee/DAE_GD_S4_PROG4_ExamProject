#pragma once
#include <SDL.h>
#include <memory>
#include <vector>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Core/Singleton.h"

namespace JRE
{
	class UIComponentBase;
	class Sprite;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class SDLRenderer final : public Singleton<SDLRenderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderSprite(AssetRef<Sprite> sprite, float x, float y) const;
		void RenderSprite(AssetRef<Sprite> sprite, float x, float y, float width, float height) const;
		void DrawRectangle(int x, int y, int w, int h, const SDL_Color& color);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
	private:
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};
	};
}

