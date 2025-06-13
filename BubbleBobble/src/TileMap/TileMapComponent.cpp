#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Physics/IPhysicsSystem.h"

#include "EngineSetup.h"
#include "TileMap/TileMapComponent.h"

using namespace JRE;

namespace BubbleBobble
{
    TileMapComponent::TileMapComponent(JRE::GameObject& gameObject)
        : RendererComponentBase(gameObject)
    {
        gameObject;
    }
    void TileMapComponent::OnEnable()
    {
        if (!m_TileMap || m_RegisteredColliders) return;

        m_StaticCollisionGruop = ServiceLocator::GetPhysicsSystem().GetFreeStaticGroup();

        for (const auto& collisionRect : m_TileMap->GetCollisionRects())
        {
            StaticCollider collider;
            collider.shape = std::make_unique<BoxShape>(collisionRect.boxShape);
            collider.group = m_StaticCollisionGruop;
            collider.properties.layer = collisionRect.isPlatform ? CollisionLayer::Platform : CollisionLayer::StaticLevel;
            collider.properties.mask = collisionRect.isPlatform ? CollisionMask::Platform : CollisionMask::StaticLevel;
            collider.properties.isStatic = true;
            ServiceLocator::GetPhysicsSystem().RegisterStaticCollider(std::move(collider));
        }
        m_RegisteredColliders = true;
    }
    void TileMapComponent::OnDisable()
    {
        if (!m_RegisteredColliders) return;
        ServiceLocator::GetPhysicsSystem().UnegisterStaticCollidersByGroup(m_StaticCollisionGruop, true);
        m_RegisteredColliders = false;
    }
    void TileMapComponent::Render() const
    {
        m_TileMap->Render();
    }
    void TileMapComponent::SetTileMap(JRE::AssetRef<TileMap> tileMap)
    {
        m_TileMap = tileMap;
    }
}
