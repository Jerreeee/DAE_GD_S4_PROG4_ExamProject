#include <fstream>
#include <sstream>
#include <iostream>
#include "SDL.h"
#undef main
#include "JREngine/Asset/Sprite.h"
#include "JREngine/Rendering/SDLRenderer.h"

#include "TileMap/TileMap.h"

using namespace JRE;

namespace BubbleBobble
{
	void TileMap::Render()
	{
		for (auto& spritePos : m_SpritePositions)
		{
			SDLRenderer::GetInstance().RenderSprite(m_Sprites[spritePos.spriteIdx], spritePos.pos.x, spritePos.pos.y);
		}

		for (const ColliderInfo& info : m_CollisionRects)
		{
			SDL_Color color = info.isPlatform ? SDL_Color{255, 0, 0, 255} : SDL_Color{255, 255, 255, 255};
			const JRE::Region& rect = info.boxShape.GetRegion();
			SDLRenderer::GetInstance().DrawRectangle(static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width), static_cast<int>(rect.height), color);
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
	void TileMap::SetCollisionRects(const std::vector<ColliderInfo>& rects)
	{
		m_CollisionRects = rects;
	}
	const std::vector<TileMap::SpritePos>& TileMap::GetDrawInfo() const
	{
		return m_SpritePositions;
	}
}
