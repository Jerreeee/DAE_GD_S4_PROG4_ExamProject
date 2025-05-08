#include "Audio/ISoundClip.h"
#include "Resources/AssetImporter.h"
#include "Resources/SoundClipImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::GetInstance().RegisterImporter(AssetType::Font, SoundClipImporter::ImportAsset);
			return true;
		}();

	SoundClipImporter::SoundClipImporter(const std::filesystem::path& filepath) :
		m_Path{ AssetImporter::GetInstance().GetFullDatapath(filepath) }
	{
	}
	Ref<Asset> SoundClipImporter::ImportAsset(AssetHandle, const AssetMetadata& metadata)
	{
		return CreateRef<ISoundClip>(metadata.filepath);
	}
	AssetMetadata SoundClipImporter::GetMetadata() const
	{
		return AssetMetadata{ AssetType::SoundClip, m_Path, "", true };
	}
}
