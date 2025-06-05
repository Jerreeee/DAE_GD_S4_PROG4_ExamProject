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
				if (worldBounds.Intersects(collisionRect.region))
				{
					// Simple vertical collision response: push collider up so it rests on the tile
					const float colliderBottom = worldBounds.Top() + worldBounds.Height();
					const float tileTop = collisionRect.region.Top();

					// If the collider is falling into the tile from above
					if (colliderBottom > tileTop)
					{
						position.y = tileTop - worldBounds.Height();
						collider->GetGameObject().SetWorldPosition(position);

						// Update bounding box for further checks if needed
						worldBounds = collider->GetWorldBounds();
					}
				}
			}
		}
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
