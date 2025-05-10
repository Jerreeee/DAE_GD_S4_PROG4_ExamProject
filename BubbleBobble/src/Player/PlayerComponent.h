#pragma once
#include "JREngine/Scene/ComponentBase.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class PlayerComponent : public JRE::ComponentBase
	{
	public:
		PlayerComponent(GameObject& gameObject);
	private:
	};
}
