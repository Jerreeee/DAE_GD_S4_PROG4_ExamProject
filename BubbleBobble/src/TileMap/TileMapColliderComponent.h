#pragma once
#include "JREngine/Asset/Asset.h"
#include "JREngine/Scene/ComponentBase.h"
#include "TileMap/TileMap.h"

namespace BubbleBobble
{
	//Component that automatically registers and unregisters itself with the Scene's TileMapPhysicsSystem
	class TileMapColliderComponent final : public JRE::ComponentBase
	{
	public:
		TileMapColliderComponent(JRE::GameObject& gameObject);
		~TileMapColliderComponent();

		virtual void Update() override {};

		const JRE::Region& GetLocalBounds() const { return m_LocalBounds; };
		JRE::Region GetWorldBounds() const;
		void SetLocalBounds(const JRE::Region& bounds) { m_LocalBounds = bounds; };
	private:
		JRE::Region m_LocalBounds;
	};
}
