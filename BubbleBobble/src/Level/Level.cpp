#include "Level.h"
#include "Level.h"
#include "Level.h"
#include "Level.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/Texture2D.h"
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Rendering/SDLRenderer.h"
#include "Level/Level.h"

namespace BubbleBobble
{
	void Level::Render()
	{
		for (auto& spritePos : m_SpritePositions)
		{
			JRE::SDLRenderer::GetInstance().RenderSprite(m_Sprites[spritePos.spriteIdx], spritePos.pos.x, spritePos.pos.y);
		}
	}
	void Level::SetSprites(const std::vector<JRE::AssetRef<JRE::Sprite>>& sprites)
	{
		m_Sprites = sprites;
	}
	void Level::SetSpritePositions(const std::vector<Level::SpritePos>& spritePositions)
	{
		m_SpritePositions = spritePositions;
	}
	const std::vector<Level::SpritePos>& Level::GetDrawInfo() const
	{
		return m_SpritePositions;
	}
}
