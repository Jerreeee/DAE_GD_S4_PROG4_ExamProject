#include "TileMap/TileMapComponent.h"

namespace BubbleBobble
{
    TileMapComponent::TileMapComponent(JRE::GameObject& gameObject)
        : RendererComponentBase(gameObject)
    {
    }
    void TileMapComponent::Render() const
    {
        m_TileMap->Render();
    }
    void TileMapComponent::SetTileMap(JRE::AssetRef<TileMap> tileMap)
    {
        m_TileMap = tileMap;
    }
}
