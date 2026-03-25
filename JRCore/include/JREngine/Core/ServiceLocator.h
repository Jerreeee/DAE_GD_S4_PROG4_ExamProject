#pragma once
#include <memory>

namespace JRE
{
	class ISoundSystem;
	class IResourceManager;
	class IPhysicsSystem;

	class ServiceLocator final
	{
	public:
		ServiceLocator();
		~ServiceLocator();

		static ISoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& pSoundSystem);

		static IResourceManager& GetResourceManager();
		static void RegisterResourceManager(std::unique_ptr<IResourceManager>&& pResourceManager);

		static IPhysicsSystem& GetPhysicsSystem();
		static void RegisterPhysicsSystem(std::unique_ptr<IPhysicsSystem>&& pPhysicsSystem);
	private:
		static std::unique_ptr<ISoundSystem> s_pSoundSystem;
		static std::unique_ptr<IResourceManager> s_pResourceManager;
		static std::unique_ptr<IPhysicsSystem> s_pPhysicsSystem;
	};
}