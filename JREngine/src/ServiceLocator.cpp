#include "ServiceLocator.h"
#include "ISoundSystem.h"
#include "IResourceManager.h"

namespace JRE
{
	std::unique_ptr<ISoundSystem> ServiceLocator::s_pSoundSystem{std::make_unique<NullSoundSystem>()};
	std::unique_ptr<IResourceManager> ServiceLocator::s_pResourceManager{ std::make_unique<NullResourceManager>() };

	ServiceLocator::ServiceLocator() = default;
	ServiceLocator::~ServiceLocator() = default;
	ISoundSystem& ServiceLocator::GetSoundSystem()
	{
		return *s_pSoundSystem;
	}
	void ServiceLocator::RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& pSoundSystem)
	{
		s_pSoundSystem = std::move(pSoundSystem);
	}
	IResourceManager& ServiceLocator::GetResourceManager()
	{
		return *s_pResourceManager;
	}
	void ServiceLocator::RegisterResourceManager(std::unique_ptr<IResourceManager>&& pResourceManager)
	{
		s_pResourceManager = std::move(pResourceManager);
	}
}
