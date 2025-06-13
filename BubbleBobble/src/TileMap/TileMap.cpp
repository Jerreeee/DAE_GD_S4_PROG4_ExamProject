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
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Physics/IPhysicsSystem.h"

#include "EngineSetup.h"
#include "TileMap/TileMap.h"

using namespace JRE;

namespace BubbleBobble
{
	TileMap::TileMap()
		: m_StaticCollisionGruop{ ServiceLocator::GetPhysicsSystem().GetFreeStaticGroup() }
	{
	}
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
		ServiceLocator::GetPhysicsSystem().UnegisterStaticCollidersByGroup(m_StaticCollisionGruop, false);

		m_CollisionRects = rects;
		for (const auto& collisionRect : m_CollisionRects)
		{
			StaticCollider collider;
			collider.shape = std::make_unique<BoxShape>(collisionRect.boxShape);
			collider.group = m_StaticCollisionGruop;
			collider.properties.layer = collisionRect.isPlatform ? CollisionLayer::Platform : CollisionLayer::StaticLevel;
			collider.properties.mask = collisionRect.isPlatform ? CollisionMask::Platform : CollisionMask::StaticLevel;
			collider.properties.isStatic = true;
			ServiceLocator::GetPhysicsSystem().RegisterStaticCollider(std::move(collider));
		}
	}
	const std::vector<TileMap::SpritePos>& TileMap::GetDrawInfo() const
	{
		return m_SpritePositions;
	}
}
