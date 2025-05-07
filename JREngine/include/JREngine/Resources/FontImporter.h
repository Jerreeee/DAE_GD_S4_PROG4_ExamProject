#pragma once
#include "JREngine/Resources/IAssetImporter.h"

namespace JRE
{
	class FontImporter : public IAssetImporter
	{
	public:
		struct ImportSettings : IAssetSpecificImportSettings
		{
			ImportSettings(uint8_t _size) : size{ _size } {};

			uint8_t size = 16;
		};

		virtual std::filesystem::path GenerateVirtualPath(const AssetImportSettings& settings) override;
		virtual Ref<Asset> Import(const AssetImportSettings& settings) override;
		virtual AssetLoadMode GetMandatoryLoadMode() const override { return AssetLoadMode::Immediate; }
		virtual std::unique_ptr<IAssetSpecificImportSettings> CreateSettingsFromArgs(const std::vector<std::any>& args) const override;
	};
}
