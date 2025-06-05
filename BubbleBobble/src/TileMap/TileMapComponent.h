#pragma once
#include "JREngine/Rendering/RendererComponentBase.h"
#include "JREngine/Asset/Asset.h"
#include "TileMap/TileMap.h"

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

		void SetTileMap(JRE::AssetRef<TileMap> tileMap);
		const TileMap& GetTileMap() const { return *m_TileMap; };
	private:
		JRE::AssetRef<TileMap> m_TileMap = nullptr;
	};
}
