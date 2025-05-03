#include "Core/ServiceLocator.h"
#include "Audio/ISoundSystem.h"

namespace JRE
{
	std::unique_ptr<ISoundSystem> ServiceLocator::s_pSoundSystem{std::make_unique<NullSoundSystem>()};

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
}
