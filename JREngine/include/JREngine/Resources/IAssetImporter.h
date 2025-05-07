#pragma once
#include <concepts>
#include <any>
#include "JREngine/Resources/Asset.h"

template<typename T>
concept ImportSettings_t = requires {
	typename T::ImportSettings;
};

namespace JRE
{
	struct IAssetSpecificImportSettings
	{
		virtual ~IAssetSpecificImportSettings() = default;
	};

	struct AssetImportSettings
	{
		std::filesystem::path path;
		AssetType assetType;
		std::unique_ptr<IAssetSpecificImportSettings> pSettings;
		AssetLoadMode loadMode = AssetLoadMode::Immediate;
		AssetHandle preGeneratedHandle;
		std::filesystem::path preGeneratedVirtualPath;
	};

	template<ImportSettings_t T, typename... Args>
	std::unique_ptr<IAssetSpecificImportSettings> CreateAssetImportSettings(Args&&... args)
	{
		return std::make_unique<typename T::ImportSettings>(std::forward<Args>(args)...);
	}

	class IAssetImporter
	{
	public:
		virtual ~IAssetImporter() = default;
		virtual std::string GenerateMetadataString(const AssetImportSettings& settings) = 0;
		virtual Ref<Asset> Import(const AssetImportSettings& settings) = 0;
		virtual AssetLoadMode GetMandatoryLoadMode() const = 0;
		virtual std::unique_ptr<IAssetSpecificImportSettings> CreateSettingsFromArgs(const std::vector<std::any>& args) const = 0;
	};
}
