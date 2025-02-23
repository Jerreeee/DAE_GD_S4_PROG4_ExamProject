#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <vector>

namespace dae
{
	class RendererComponentBase;
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();
		void Cleanup();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

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

