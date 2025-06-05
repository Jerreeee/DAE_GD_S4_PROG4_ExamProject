#include <stdexcept>
#include <cstring>
#include <iostream>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Rendering/SDLRenderer.h"
#include "Asset/Texture2D.h"
#include "Asset/Sprite.h"
#include "Asset/ResourceManager.h"
#include "Rendering/RendererComponentBase.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void JRE::SDLRenderer::Init(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Initialize SDL2 ImGui backend
	ImGui_ImplSDL2_InitForSDLRenderer(window, m_Renderer);
	ImGui_ImplSDLRenderer2_Init(m_Renderer);
}

void JRE::SDLRenderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	const auto& renderComponents = SceneManager::GetInstance().GetCurrentScene().GetRenderComponents();
	for (const auto pRendererComponent : renderComponents)
		if (pRendererComponent->IsActive())
			pRendererComponent->Render();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Renderer);

	SDL_RenderPresent(m_Renderer);
}

void JRE::SDLRenderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void JRE::SDLRenderer::RenderSprite(AssetRef<Sprite> sprite, const float x, const float y) const
{
	if (!sprite)
	{
		std::cerr << "SDLRenderer::RenderTexture() | Ref<Asset> asset was nullptr\n";
		return;
	}

	const AssetRef<Texture2D>& pTexture = sprite->GetTexture();
	SDL_Texture* pSDLTexture = pTexture->GetSDLTexture();
	const Region& region = sprite->GetSrcRegion();
	SDL_Rect src{ static_cast<int>(region.x), static_cast<int>(region.y), static_cast<int>(region.width), static_cast<int>(region.height) };

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = src.w;
	dst.h = src.h;
	SDL_RenderCopy(GetSDLRenderer(), pSDLTexture, &src, &dst);
}

void JRE::SDLRenderer::RenderSprite(AssetRef<Sprite> sprite, const float x, const float y, const float width, const float height) const
{
	if (!sprite)
	{
		std::cerr << "SDLRenderer::RenderTexture() | Ref<Asset> asset was nullptr\n";
		return;
	}

	const AssetRef<Texture2D>& pTexture = sprite->GetTexture();
	SDL_Texture* pSDLTexture = pTexture->GetSDLTexture();
	const Region& region = sprite->GetSrcRegion();
	SDL_Rect src{ static_cast<int>(region.x), static_cast<int>(region.y), static_cast<int>(region.width), static_cast<int>(region.height) };

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), pSDLTexture, &src, &dst);
}

void JRE::SDLRenderer::DrawRectangle(int x, int y, int w, int h, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(m_Renderer, &rect);
}

SDL_Renderer* JRE::SDLRenderer::GetSDLRenderer() const { return m_Renderer; }
