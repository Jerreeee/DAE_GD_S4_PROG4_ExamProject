	#pragma once
	#include <memory>
	#include "JREngine/Audio/ISoundSystem.h"

	namespace JRE
	{
		using SoundHandle = uint16_t;
		class SDLSoundSystem final : public ISoundSystem
		{
		public:
			SDLSoundSystem();
			~SDLSoundSystem();

			virtual AssetRef<ISoundClip> CreateSoundClip(AssetHandle handle, const AssetMetadata& metadata) override;
			virtual void Play(AssetRef<ISoundClip> soundClipAsset, float volume = 1.0f) override;

			virtual AssetRef<ISoundMusic> CreateMusic(AssetHandle handle, const AssetMetadata& metadata) override;
			virtual void PlayMusic(AssetRef<ISoundMusic> music, bool loop = true, float volume = 1.0f) override;
			virtual void StopMusic() override;
		private:
			class Impl;
			std::unique_ptr<Impl> m_pImpl{};
		};
	}
