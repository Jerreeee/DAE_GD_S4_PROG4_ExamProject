#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Scene/Scene.h"

namespace BubbleBobble
{
	class TileMapColliderComponent;
	class TileMap;
	class TileMapPhysicsSystem final : public JRE::ISceneSystem
	{
	public:
		TileMapPhysicsSystem(const TileMap& tileMap);

		virtual void Update(JRE::Scene& scene) override;

		void RegisterCollider(TileMapColliderComponent* comp);
		void UnregisterCollider(TileMapColliderComponent* comp);
	private:
		const TileMap* m_pTileMap;
		std::vector<TileMapColliderComponent*> m_Components;
	};
}
