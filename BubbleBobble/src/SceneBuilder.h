#pragma once
#include <filesystem>
#include "JREngine/Scene/Scene.h"

namespace BubbleBobble
{
	class SceneBuilder final
	{
	public:
		SceneBuilder(const std::filesystem::path& sceneDescriptionPath);
		void Build(JRE::Scene& scene);
	private:
		void AddTileMap();
		
		JRE::Scene* m_pScene{ nullptr };
		std::filesystem::path m_SceneDescriptionPath{};
	};
}
