#pragma once
#include <filesystem>
#include <string>
#include "JREngine/Core/TypeRegistry.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	struct AssetImporterTypeInfo
	{
		const std::string& name;
	};

	class IAssetImporter
	{
	public:
		virtual ~IAssetImporter() = default;
		virtual AssetMetadata GetMetadata() const = 0;
	};
}
