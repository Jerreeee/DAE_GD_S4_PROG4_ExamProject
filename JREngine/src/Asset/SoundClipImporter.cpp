#include "Core/ServiceLocator.h"
#include "Audio/ISoundClip.h"
#include "Audio/ISoundSystem.h"
#include "Asset/SoundClipImporter.h"

namespace JRE
{
	// Loader registration is done explicitly at engine init (see RegisterBuiltinAssetLoaders
	// in JREngine.cpp), not via a static initializer here — those get stripped from the static lib.

	SoundClipImporter::SoundClipImporter(const std::filesystem::path& filepath) :
		m_Path{ filepath }
	{
	}
	AssetRef<Asset> SoundClipImporter::Load(AssetHandle handle, const AssetMetadata& metadata)
	{
		return ServiceLocator::GetSoundSystem().CreateSoundClip(handle, metadata);
	}
	AssetMetadata SoundClipImporter::GetMetadata() const
	{
		return AssetMetadata{ ISoundClip::GetStaticType().data(), m_Path, "", true, GetDeclaredDependencies() };
	}
}
