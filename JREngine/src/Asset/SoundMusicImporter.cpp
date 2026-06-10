#include "Core/ServiceLocator.h"
#include "Audio/ISoundMusic.h"
#include "Audio/ISoundSystem.h"
#include "Asset/SoundMusicImporter.h"

namespace JRE
{
	// Loader registration is done explicitly at engine init (see RegisterBuiltinAssetLoaders
	// in JREngine.cpp), not via a static initializer here — those get stripped from the static lib.

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
