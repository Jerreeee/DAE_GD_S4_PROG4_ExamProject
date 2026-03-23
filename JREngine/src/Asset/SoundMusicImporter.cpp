#include "Core/ServiceLocator.h"
#include "Audio/ISoundMusic.h"
#include "Audio/ISoundSystem.h"
#include "Asset/AssetLoaderRegistry.h"
#include "Asset/SoundMusicImporter.h"

namespace JRE
{
	static bool s_RegisteredMusic = []()
		{
			AssetLoaderRegistry::GetInstance().RegisterLoader(
				ISoundMusic::GetStaticType(),
				SoundMusicImporter::Load
			);
			return true;
		}();

	SoundMusicImporter::SoundMusicImporter(const std::filesystem::path& filepath)
		: m_Path{ filepath }
	{
	}

	AssetRef<Asset> SoundMusicImporter::Load(AssetHandle handle, const AssetMetadata& metadata)
	{
		return ServiceLocator::GetSoundSystem().CreateMusic(handle, metadata);
	}

	AssetMetadata SoundMusicImporter::GetMetadata() const
	{
		return AssetMetadata{ISoundMusic::GetStaticType().data(), m_Path, "", true, GetDeclaredDependencies()};
	}
}
