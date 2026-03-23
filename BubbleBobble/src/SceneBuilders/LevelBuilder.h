#pragma once
#include "JREngine/Scene/Scene.h"

namespace BubbleBobble
{
	class LevelBuilder final
	{
	public:
		LevelBuilder(JRE::Scene& scene, int levelIdx);
		void Build();
	private:
		void AddTileMap();
		void AddLevelData();
		void AddPortal();

		JRE::Scene& m_Scene;
		int m_LevelIdx{};
	};
}
