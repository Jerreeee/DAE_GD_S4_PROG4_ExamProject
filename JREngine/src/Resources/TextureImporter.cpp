#include "Rendering/Texture2D.h"
#include "Resources/AssetImporter.h"
#include "Resources/TextureImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::RegisterImporter(AssetType::Texture2D, CreateRef<TextureImporter>());
			return true;
		}();

	std::filesystem::path TextureImporter::GenerateVirtualPath(const AssetImportSettings& settings)
	{
		std::stringstream ss{};
		ss << "Texture2D/" << settings.path;
		return std::filesystem::path(ss.str());
	}

	Ref<Asset> TextureImporter::Import(const AssetImportSettings& settings)
	{
		return CreateRef<Texture2D>(settings.path.string());
	}
}
