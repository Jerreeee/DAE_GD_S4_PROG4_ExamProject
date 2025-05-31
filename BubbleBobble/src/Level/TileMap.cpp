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
#include "Level/TileMap.h"

namespace BubbleBobble
{
	void TileMap::Render()
	{
		for (auto& spritePos : m_SpritePositions)
		{
			JRE::SDLRenderer::GetInstance().RenderSprite(m_Sprites[spritePos.spriteIdx], spritePos.pos.x, spritePos.pos.y);
		}
	}
	void TileMap::SetSprites(const std::vector<JRE::AssetRef<JRE::Sprite>>& sprites)
	{
		m_Sprites = sprites;
	}
	void TileMap::SetSpritePositions(const std::vector<TileMap::SpritePos>& spritePositions)
	{
		m_SpritePositions = spritePositions;
	}
	void TileMap::SetCollisionRects(const std::vector<glm::vec4>& rects)
	{
		m_CollisionRects = rects;
	}
	const std::vector<TileMap::SpritePos>& TileMap::GetDrawInfo() const
	{
		return m_SpritePositions;
	}
}
