#pragma once
#include "JREngine/Rendering/RendererComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "Level/TileMap.h"

namespace JRE
{
	class GameObject;
}

namespace BubbleBobble
{
	class TileMapComponent final : public JRE::RendererComponentBase
	{
	public:
		TileMapComponent(JRE::GameObject& gameObject);

		virtual void Render() const;

		void SetLevel(JRE::AssetRef<TileMap> level);
	private:
		JRE::AssetRef<TileMap> m_Level = nullptr;
	};
}
