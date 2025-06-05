#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"

namespace JRE
{
	struct Region;
}

namespace BubbleBobble
{
	class TileMapColliderComponent;
	class TileMap;
	class TileMapPhysicsSystem final : public JRE::ISceneSystem
	{
	public:
		TileMapPhysicsSystem(const TileMap& tileMap);

		virtual void Update(JRE::Scene& scene) override;

		bool IsRectOverlappingCollider(const JRE::Region& rect) const;

		void RegisterCollider(TileMapColliderComponent* comp);
		void UnregisterCollider(TileMapColliderComponent* comp);
	private:
		const TileMap* m_pTileMap;
		std::vector<TileMapColliderComponent*> m_Components;
	};
}
