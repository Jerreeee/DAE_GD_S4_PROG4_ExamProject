#include "JREngine/Scene/SceneManager.h"

#include "TileMap/TileMapPhysicsSystem.h"
#include "JREngine/Scene/Scene.h"
#include "TileMap/TileMapColliderComponent.h"

using namespace JRE;

namespace BubbleBobble
{
    TileMapColliderComponent::TileMapColliderComponent(JRE::GameObject& gameObject)
		: ComponentBase(gameObject)
    {
		auto& sm = SceneManager::GetInstance();
		auto* system = sm.GetCurrentScene().GetSystem<TileMapPhysicsSystem>();
		if (system)
			system->RegisterCollider(this);
    }
	TileMapColliderComponent::~TileMapColliderComponent()
	{
		auto& sm = SceneManager::GetInstance();
		if (auto* system = sm.GetCurrentScene().GetSystem<TileMapPhysicsSystem>())
			system->UnregisterCollider(this);
	}
	JRE::Region TileMapColliderComponent::GetWorldBounds() const
	{
		auto& transform = GetGameObject();
		glm::vec3 pos = transform.GetWorldPosition();
		float x = pos.x + m_LocalBounds.x;
		float y = pos.y + m_LocalBounds.y;
		return JRE::Region{ x, y, m_LocalBounds.width, m_LocalBounds.height };
	}
}
