#pragma once
#include <filesystem>
#include <string>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	class IAssetImporter
	{
	public:
		virtual ~IAssetImporter() = default;
		virtual AssetMetadata GetMetadata() const = 0;
	};
}
