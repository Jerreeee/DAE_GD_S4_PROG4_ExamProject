#pragma once
#include "SDL.h"
#undef main
#include "JREngine/Scene/Scene.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Font.h"

namespace BubbleBobble
{
	class BuilderHelpers final
	{
	public:
		static void AddCenteredTxt(JRE::Scene& scene, const std::string& text, JRE::SoftAssetRef<JRE::Font> font, float centerX, float y, SDL_Color color);
	};

	class MainMenuBuilder final
	{
	public:
		MainMenuBuilder(JRE::Scene& scene);
		void Build();
	private:
		JRE::Scene& m_Scene;
	};

	class LoadingMenuBuilder final
	{
	public:
		LoadingMenuBuilder(JRE::Scene& scene);
		void Build();
	private:
		JRE::Scene& m_Scene;
	};
}
