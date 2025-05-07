#include "Audio/ISoundClip.h"
#include "Resources/AssetImporter.h"
#include "Resources/SoundClipImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::RegisterImporter(AssetType::Font, CreateRef<SoundClipImporter>());
			return true;
		}();

	std::filesystem::path SoundClipImporter::GenerateVirtualPath(const AssetImportSettings& settings)
	{
		std::stringstream ss{};
		ss << "SoundClip/" << settings.path;
		return std::filesystem::path(ss.str());
	}

	Ref<Asset> SoundClipImporter::Import(const AssetImportSettings& settings)
	{
		return CreateRef<ISoundClip>(settings.path);
	}
}
