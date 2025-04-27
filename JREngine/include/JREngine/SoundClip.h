	#pragma once

	struct Mix_Chunk;

	namespace JRE
	{
		class SoundClip
		{
		public:
			explicit SoundClip(const std::string& filePath);
			~SoundClip();

			SoundClip(const SoundClip&) = delete;
			SoundClip& operator=(const SoundClip&) = delete;
			SoundClip(SoundClip&& other) noexcept;
			SoundClip& operator=(SoundClip&& other) noexcept;

			void Play(int loops = 0, int channel = -1) const;
			void SetVolume(float volume);
		private:
			Mix_Chunk* m_pChunk = nullptr;
		};
	}