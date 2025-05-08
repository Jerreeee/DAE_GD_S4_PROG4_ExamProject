#pragma once
#include <filesystem>
#include <string>
#include "JREngine/Resources/Asset.h"
#include "JREngine/Resources/AssetMetadata.h"

namespace JRE
{
	class IAssetImporter
	{
	public:
		virtual ~IAssetImporter() = default;
		virtual AssetMetadata GetMetadata() const = 0;
	};
}
