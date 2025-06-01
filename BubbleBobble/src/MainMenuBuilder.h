#pragma once
#include "JREngine/Scene/Scene.h"
#include "JREngine/Asset/SoftAssetRef.h"

namespace BubbleBobble
{
	class MainMenuBuilder final
	{
	public:
		MainMenuBuilder(JRE::Scene& scene);
		void Build();
	private:
		JRE::Scene& m_Scene;
		void AddTextAtPos(const std::string& text, JRE::SoftAssetRef<JRE::Font> font, float y);
	};
}
