#include "Audio/ISoundSystem.h"
#include "Asset/IResourceManager.h"
#include "Physics/IPhysicsSystem.h"
#include "Core/ServiceLocator.h"

namespace JRE
{
	std::unique_ptr<ISoundSystem> ServiceLocator::s_pSoundSystem{ std::make_unique<NullSoundSystem>() };
	std::unique_ptr<IResourceManager> ServiceLocator::s_pResourceManager{ std::make_unique<NullResourceManager>() };
	std::unique_ptr<IPhysicsSystem> ServiceLocator::s_pPhysicsSystem{ std::make_unique<NullPhysicsSystem>() };
	std::unique_ptr<TypeRegistry<AssetTypeInfo>> ServiceLocator::s_pAssetTypeRegistry{ std::make_unique<TypeRegistry<AssetTypeInfo>>() };

	ServiceLocator::ServiceLocator() = default;
	ServiceLocator::~ServiceLocator() = default;
	ISoundSystem& ServiceLocator::GetSoundSystem()
	{
		return *s_pSoundSystem;
	}
	void ServiceLocator::RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& pSoundSystem)
	{
		s_pSoundSystem = pSoundSystem ? std::move(pSoundSystem) : std::make_unique<NullSoundSystem>();
	}
	IResourceManager& ServiceLocator::GetResourceManager()
	{
		return *s_pResourceManager;
	}
	void ServiceLocator::RegisterResourceManager(std::unique_ptr<IResourceManager>&& pResourceManager)
	{
		s_pResourceManager = pResourceManager ? std::move(pResourceManager) : std::make_unique<NullResourceManager>();
	}
	IPhysicsSystem& ServiceLocator::GetPhysicsSystem()
	{
		return *s_pPhysicsSystem;
	}
	void ServiceLocator::RegisterPhysicsSystem(std::unique_ptr<IPhysicsSystem>&& pPhysicsSystem)
	{
		s_pPhysicsSystem = pPhysicsSystem ? std::move(pPhysicsSystem) : std::make_unique<NullPhysicsSystem>();
	}
	TypeRegistry<AssetTypeInfo>& ServiceLocator::GetAssetTypeRegistry()
	{
		return *s_pAssetTypeRegistry;
	}
}
