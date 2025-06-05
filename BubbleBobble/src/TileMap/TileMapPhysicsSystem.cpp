#include "TileMap/TileMapColliderComponent.h"
#include "TileMap/TileMapPhysicsSystem.h"

namespace BubbleBobble
{
	TileMapPhysicsSystem::TileMapPhysicsSystem(const TileMap& tileMap)
		: m_pTileMap{ &tileMap }
	{
	}
	void TileMapPhysicsSystem::Update(JRE::Scene&)
	{
		if (!m_pTileMap)
			return;

		const auto& collisionRects = m_pTileMap->GetCollisionRects();

		for (auto* collider : m_Components)
		{
			if (!collider)
				continue;

			JRE::Region worldBounds = collider->GetWorldBounds();
			glm::vec3 position = collider->GetGameObject().GetWorldPosition();

			for (const auto& collisionRect : collisionRects)
			{
				if (!worldBounds.Intersects(collisionRect.region))
					continue;

				const float aLeft = worldBounds.Left();
				const float aRight = worldBounds.Right();
				const float aTop = worldBounds.Top();
				const float aBottom = worldBounds.Bottom();

				const float bLeft = collisionRect.region.Left();
				const float bRight = collisionRect.region.Right();
				const float bTop = collisionRect.region.Top();
				const float bBottom = collisionRect.region.Bottom();

				const float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
				const float overlapY = std::min(aBottom, bBottom) - std::max(aTop, bTop);

				if (overlapX > 0.0f)
				{
					if (aRight > bLeft && aLeft < bLeft)
						position.x -= overlapX; // from left
					else if (aLeft < bRight && aRight > bRight)
						position.x += overlapX; // from right
				}

				if (overlapY > 0.0f)
				{
					if (aBottom > bTop && aTop < bTop)
						position.y -= overlapY; // from top
					else if (aTop < bBottom && aBottom > bBottom)
						position.y += overlapY; // from bottom
				}

				//Set fixed position and recalculate bounds
				collider->GetGameObject().SetWorldPosition(position);
				worldBounds = collider->GetWorldBounds();
			}
		}
	}
	bool TileMapPhysicsSystem::IsRectOverlappingCollider(const JRE::Region& rect) const
	{
		if (!m_pTileMap)
			return false;

		for (const auto& tile : m_pTileMap->GetCollisionRects())
			if (rect.Intersects(tile.region))
				return true;
		return false;
	}
	void TileMapPhysicsSystem::RegisterCollider(TileMapColliderComponent* comp)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), comp);
		if (it != m_Components.end())
			return;
		m_Components.emplace_back(comp);
	}
	void TileMapPhysicsSystem::UnregisterCollider(TileMapColliderComponent* comp)
	{
		auto it = std::remove(m_Components.begin(), m_Components.end(), comp);
		if (it != m_Components.end())
			m_Components.erase(it, m_Components.end());
	}
}
