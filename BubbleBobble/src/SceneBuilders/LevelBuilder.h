#pragma once
#include <filesystem>
#include "JREngine/Scene/Scene.h"

namespace BubbleBobble
{
	class LevelBuilder final
	{
	public:
		LevelBuilder(JRE::Scene& scene, const std::filesystem::path& path);
		void Build();
	private:
		void AddTileMap();
		
		JRE::Scene& m_Scene;
		std::filesystem::path m_Path{};
	};
}
