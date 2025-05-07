#pragma once
#include "JREngine/Resources/IAssetImporter.h"

namespace JRE
{
	class SoundClipImporter : public IAssetImporter
	{
	public:
		virtual std::filesystem::path GenerateVirtualPath(const AssetImportSettings& settings) override;
		virtual Ref<Asset> Import(const AssetImportSettings& settings) override;
		virtual AssetLoadMode GetMandatoryLoadMode() const override { return AssetLoadMode::Immediate; }
	};
}
