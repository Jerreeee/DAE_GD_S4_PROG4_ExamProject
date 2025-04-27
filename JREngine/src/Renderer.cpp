#include <stdexcept>
#include <cstring>
#include "SceneManager.h"
#include "Texture2D.h"
#include "RendererComponentBase.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Renderer.h"

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

void JRE::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// Initialize SDL2 ImGui backend
	ImGui_ImplSDL2_InitForSDLRenderer(window, m_renderer);
	ImGui_ImplSDLRenderer2_Init(m_renderer);
}

void JRE::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	for (const auto pRendererComponent : m_RendererComponents)
		if (pRendererComponent->IsActive())
			pRendererComponent->Render();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_renderer);

	SDL_RenderPresent(m_renderer);
}

void JRE::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void JRE::Renderer::RenderTexture(const std::shared_ptr<Texture2D>& pTexture, const float x, const float y) const
{
	if (!pTexture) return;

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(pTexture->GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), pTexture->GetSDLTexture(), nullptr, &dst);
}

void JRE::Renderer::RenderTexture(const std::shared_ptr<Texture2D>& pTexture, const float x, const float y, const float width, const float height) const
{
	if (!pTexture) return;

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), pTexture->GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* JRE::Renderer::GetSDLRenderer() const { return m_renderer; }

void JRE::Renderer::RegisterRendererComponent(RendererComponentBase* pRendererComponent)
{
	m_RendererComponents.emplace_back(pRendererComponent);
}

void JRE::Renderer::UnRegisterRendererComponent(RendererComponentBase* pRendererComponent)
{
	m_RendererComponents.erase(std::find(m_RendererComponents.begin(), m_RendererComponents.end(), pRendererComponent));
}
