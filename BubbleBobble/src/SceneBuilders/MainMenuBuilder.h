#pragma once
#include "JREngine/Scene/Scene.h"

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
