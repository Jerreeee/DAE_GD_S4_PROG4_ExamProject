#include "Resources/Font.h"
#include "Resources/AssetImporter.h"
#include "Resources/FontImporter.h"

namespace JRE
{
	static bool s_Registered = []()
		{
			AssetImporter::RegisterImporter(AssetType::Font, CreateRef<FontImporter>());
			return true;
		}();

	std::filesystem::path FontImporter::GenerateVirtualPath(const AssetImportSettings& settings)
	{
		auto fontSettings = static_cast<const ImportSettings*>(settings.pSettings.get());
		uint8_t size = fontSettings->size;
		std::stringstream ss{};
		ss << "Font/" << settings.path << '@' << std::to_string(size);
		return std::filesystem::path(ss.str());
	}

	Ref<Asset> FontImporter::Import(const AssetImportSettings& settings)
	{
		auto fontSettings = static_cast<const ImportSettings*>(settings.pSettings.get());
		uint8_t size = fontSettings->size;
		return CreateRef<Font>(settings.path, size);
	}
	std::unique_ptr<IAssetSpecificImportSettings> FontImporter::CreateSettingsFromArgs(const std::vector<std::any>& args) const
	{
		if (args.size() != 1 || !args[0].has_value() || args[0].type() != typeid(int))
			throw std::invalid_argument("FontImporter expects 1 int argument (font size)");

		int size = std::any_cast<int>(args[0]);
		return std::make_unique<ImportSettings>(static_cast<uint8_t>(size));
	}
}
