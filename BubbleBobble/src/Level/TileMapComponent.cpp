#include "Level/TileMapComponent.h"

namespace BubbleBobble
{
    TileMapComponent::TileMapComponent(JRE::GameObject& gameObject)
        : RendererComponentBase(gameObject)
    {
    }
    void TileMapComponent::Render() const
    {
        m_Level->Render();
    }
    void TileMapComponent::SetLevel(JRE::AssetRef<TileMap> level)
    {
        m_Level = level;
    }
}
