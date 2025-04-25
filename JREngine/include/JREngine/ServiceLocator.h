#pragma once
#include <memory>

namespace JREngine
{
	class ISoundSystem;
	class ServiceLocator final
	{
	public:
		ServiceLocator();
		~ServiceLocator();

		static ISoundSystem& GetSoundSystem()
		{
			return *s_pSoundSystem;
		}
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& pSoundSystem)
		{
			s_pSoundSystem = std::move(pSoundSystem);
		}
	private:
		inline static std::unique_ptr<ISoundSystem> s_pSoundSystem{};
	};
}