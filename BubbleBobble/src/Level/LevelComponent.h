#pragma once
#include "JREngine/Rendering/RendererComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "Level/Level.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class LevelComponent final : public JRE::RendererComponentBase
	{
	public:
		LevelComponent(JRE::GameObject& gameObject);

		virtual void Render() const;

		void SetLevel(JRE::AssetRef<Level> level);
	private:
		JRE::AssetRef<Level> m_Level = nullptr;
	};
}
