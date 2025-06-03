#pragma once
#include <string>
#include "JREngine/Asset/Asset.h"
#include "glm.hpp"

namespace BubbleBobble
{
	enum class EnemyType { Zenchan, Maita };

	struct EnemyInfo
	{
		glm::vec2 pos;
		EnemyType type;
	};

	struct LevelData : public JRE::Asset
	{
		std::vector<glm::vec2> players;
		std::vector<EnemyInfo> enemies;

		static constexpr std::string_view GetStaticType() { return "BubbleBobble::LevelData"; };
		virtual std::string_view GetType() const { return GetStaticType(); };
	};
}
