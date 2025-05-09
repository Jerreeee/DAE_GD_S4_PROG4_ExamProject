#include "Core/ServiceLocator.h"
#include "Audio/ISoundClip.h"
#include "Audio/ISoundSystem.h"
#include "Resources/AssetImporter.h"
#include "Resources/SoundClipImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::GetInstance().RegisterImporter(AssetType::SoundClip, SoundClipImporter::ImportAsset);
			return true;
		}();

	SoundClipImporter::SoundClipImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporter::GetInstance().GetFullDatapath(filepath) }
	{
	}
	Ref<Asset> SoundClipImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		return ServiceLocator::GetSoundSystem().CreateSoundClip(handle, metadata);
	}
	AssetMetadata SoundClipImporter::GetMetadata() const
	{
		return AssetMetadata{ AssetType::SoundClip, m_Path, "", true };
	}
}
