#include <fstream>
#include <sstream>
#include <iostream>
#include "SDL.h"
#undef main
#include "JREngine/Asset/SoftAssetRef.h"
#include "JREngine/Asset/ResourceManager.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/Texture2D.h"
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
			const JRE::Region& rect = info.region;
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
	bool TileMap::MovePosition(const JRE::Region& oldRegion, glm::vec2 vel, float dt, bool applyGravity, CollisionInfo& ci) const
	{
		glm::vec2 oldPos{ oldRegion.x, oldRegion.y };

		if (applyGravity)
			vel.y += m_Gravity * dt;

		glm::vec2 endPos{
			oldPos.x + vel.x * m_PixPerM * dt,
			oldPos.y + vel.y * m_PixPerM * dt,
		};

		VelInfo vi{ GetVelInfo(vel) };
		ci.velOut = vel;

		Region newRegionX{ endPos.x, oldPos.y, oldRegion.width, oldRegion.height };
		Region newRegionY{ oldPos.x, endPos.y, oldRegion.width, oldRegion.height };
		glm::vec2 absOffset{ std::abs(endPos.x - oldPos.x), std::abs(endPos.y - oldPos.y) };

		for (const auto& collider : m_CollisionRects)
		{
			const Region& collRegion = collider.region;

			bool alreadyCollidingY = oldRegion.OverlapInY(collRegion);
			bool skipPlatFormDownColl = collider.isPlatform && alreadyCollidingY;

			//Check X collision
			if (!collider.isPlatform && vi.InX() && newRegionX.Intersects(collRegion))
			{
				if (vi.left && newRegionX.Left() < collRegion.Right()) //Colliding left
				{
					float lambda = (oldRegion.Left() - collRegion.Right()) / absOffset.x;
					if (lambda < ci.left.lambda)
					{
						ci.collDir.left = true;
						ci.left.lambda = lambda;
						ci.left.collPos = collRegion.Right();
						ci.left.entryDist = collRegion.Right() - newRegionX.Left();
					}
				}
				else if (vi.right && collRegion.Left() < newRegionX.Right()) //Colliding Right
				{
					float lambda = (collRegion.Left() - oldRegion.Right()) / absOffset.x;
					if (lambda < ci.right.lambda)
					{
						ci.collDir.right = true;
						ci.right.lambda = lambda;
						ci.right.collPos = collRegion.Left();
						ci.right.entryDist = newRegionX.Right() - collRegion.Left();
					}
				}
			}

			//Check Y collision
			if (vi.InY() && newRegionY.Intersects(collRegion))
			{
				if (!collider.isPlatform && vi.up && newRegionY.Top() < collRegion.Bottom()) //Colliding up
				{
					float lambda = (oldRegion.Top() - collRegion.Bottom()) / absOffset.y;
					if (lambda < ci.up.lambda)
					{
						ci.collDir.up = true;
						ci.up.lambda = lambda;
						ci.up.collPos = collRegion.Bottom();
						ci.up.entryDist = collRegion.Bottom() - newRegionY.Top();
					}
				}
				else if (vi.down && collRegion.Top() < newRegionY.Bottom() && !skipPlatFormDownColl) //Colliding down
				{
					float lambda = (collRegion.Top() - oldRegion.Bottom()) / absOffset.y;
					if (lambda < ci.down.lambda)
					{
						ci.collDir.down = true;
						ci.down.lambda = lambda;
						ci.down.collPos = collRegion.Top();
						ci.down.entryDist = newRegionY.Bottom() - collRegion.Top();
					}
				}
			}
		}	

		if (!ci.Collided())
			ci.newPos = endPos;
		else if (ci.collDir.X() && !ci.collDir.Y()) //Collided in X only
		{
			float x = ci.collDir.right ? ci.right.collPos - oldRegion.width : ci.left.collPos;
			ci.newPos = glm::vec2{ x, endPos.y };
			ci.velOut.x = 0.f;
		}
		else if (ci.collDir.Y() && !ci.collDir.X()) //Collided in Y only
		{
			float y = ci.collDir.up ? ci.up.collPos : ci.down.collPos - oldRegion.height;
			ci.newPos = glm::vec2{ endPos.x, y };
			ci.velOut.y = 0.f;
		}
		else if (vi.LeftDown() && ci.collDir.LeftBottom()) //LeftBottom concave collision
		{
			ci.newPos = glm::vec2{ ci.left.collPos, ci.down.collPos - oldRegion.height };
			ci.velOut = glm::vec2{};
		}
		else if (vi.LeftUp() && ci.collDir.LeftTop()) //LeftTop concave collision
		{
			ci.newPos = glm::vec2{ ci.left.collPos, ci.up.collPos };
			ci.velOut = glm::vec2{};
		}
		else if (vi.RightUp() && ci.collDir.RightTop()) //RightTop concave collision
		{
			ci.newPos = glm::vec2{ ci.right.collPos - oldRegion.width, ci.up.collPos };
			ci.velOut = glm::vec2{};
		}
		else if (vi.RightDown() && ci.collDir.RightBottom()) //RightBottom concave collision
		{
			ci.newPos = glm::vec2{ ci.right.collPos - oldRegion.width, ci.down.collPos - oldRegion.height };
			ci.velOut = glm::vec2{};
		}
		else //convex corner collision
		{
			//Calculate if the collision happened in X or Y first
			float lambdaX{ (vi.right) ? ci.right.lambda : ci.left.lambda };
			float lambdaY{ (vi.up) ? ci.up.lambda : ci.down.lambda };
			if (lambdaY <= lambdaX) //Y wins when equal to X
			{
				float y = ci.collDir.up ? ci.up.collPos : ci.down.collPos - oldRegion.height;
				ci.newPos = glm::vec2{ endPos.x, y };
				ci.collDir.left = false;
				ci.collDir.right = false;
				ci.velOut.y = 0.f;
			}
			else //lambdaX > lambdaY
			{
				float x = ci.collDir.right ? ci.right.collPos - oldRegion.width : ci.left.collPos;
				ci.newPos = glm::vec2{ x, endPos.y };
				ci.collDir.up = false;
				ci.collDir.down = false;
				ci.velOut.x = 0.f;
			}
		}

		return ci.Collided();
	}
	const std::vector<TileMap::SpritePos>& TileMap::GetDrawInfo() const
	{
		return m_SpritePositions;
	}
	VelInfo TileMap::GetVelInfo(glm::vec2 vel) const
	{
		VelInfo vi{};
		vi.left = vel.x < 0.f;
		vi.right = !vi.left;
		vi.up = vel.y < 0.f;
		vi.down = !vi.up;
		return vi;
	}
}
