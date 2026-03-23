#include "Core/ServiceLocator.h"
#include "Audio/ISoundClip.h"
#include "Audio/ISoundSystem.h"
#include "Asset/AssetLoaderRegistry.h"
#include "Asset/SoundClipImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetLoaderRegistry::GetInstance().RegisterLoader(ISoundClip::GetStaticType(), SoundClipImporter::Load);
			return true;
		}();

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
