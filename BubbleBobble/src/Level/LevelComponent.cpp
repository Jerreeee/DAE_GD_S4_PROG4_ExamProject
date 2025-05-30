#include "Level/LevelComponent.h"

namespace BubbleBobble
{
    LevelComponent::LevelComponent(JRE::GameObject& gameObject)
        : RendererComponentBase(gameObject)
    {
    }
    void LevelComponent::Render() const
    {
        m_Level->Render();
    }
    void LevelComponent::SetLevel(JRE::AssetRef<Level> level)
    {
        m_Level = level;
    }
}
