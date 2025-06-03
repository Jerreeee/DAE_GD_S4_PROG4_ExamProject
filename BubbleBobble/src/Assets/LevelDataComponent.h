#pragma once
#include "JREngine/Scene/ComponentBase.h"
#include "JREngine/Asset/Asset.h"

#include "Assets/LevelData.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class LevelDataComponent : public JRE::ComponentBase
	{
	public:
		LevelDataComponent(JRE::GameObject& gameObject)
			: JRE::ComponentBase(gameObject)
		{
		}

		virtual void Update() override {};

		JRE::AssetRef<LevelData> m_LevelData = nullptr;
	};
}
