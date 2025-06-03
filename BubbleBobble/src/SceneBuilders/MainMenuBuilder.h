#pragma once
#include "JREngine/Scene/Scene.h"
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/Font.h"

namespace BubbleBobble
{


	class MainMenuBuilder final
	{
	public:
		MainMenuBuilder(JRE::Scene& scene);
		void Build();
	private:
		JRE::Scene& m_Scene;
	};
}
