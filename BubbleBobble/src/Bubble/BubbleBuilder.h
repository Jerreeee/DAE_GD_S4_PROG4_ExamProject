#pragma once
#include "JREngine/Scene/GameObject.h"
#include "glm.hpp"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class BubbleBuilder final
	{
	public:
		std::unique_ptr<JRE::GameObject> Build(const std::string& playerName, const glm::vec2& position, int direction);
	};
}
