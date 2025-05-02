#pragma once
#include <SDL.h>
#include <memory>
#include <vector>
#include "JREngine/Core/Singleton.h"

namespace JRE
{
	class RendererComponentBase;
	class UIComponentBase;
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class SDLRenderer final : public Singleton<SDLRenderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const std::shared_ptr<Texture2D>& pTexture, float x, float y) const;
		void RenderTexture(const std::shared_ptr<Texture2D>& pTexture, float x, float y, float width, float height) const;
		//void RenderTexture(const std::shared_ptr<Texture2D>& pTexture, float x, float y, )

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

		void RegisterRendererComponent(RendererComponentBase* pRendererComponent);
		void UnRegisterRendererComponent(RendererComponentBase* pRendererComponent);
	private:
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};

		std::vector<RendererComponentBase*> m_RendererComponents{};
	};
}

