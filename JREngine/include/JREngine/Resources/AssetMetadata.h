#pragma once
#include <filesystem>
#include "JREngine/Resources/Asset.h"

namespace JRE
{
	struct AssetMetadata
	{
		inline std::filesystem::path GetVirtualPath()
		{
			return std::filesystem::path(filepath / uniqueID);
		}

		AssetType assetType{ AssetType::None };
		std::filesystem::path filepath;
		std::string uniqueID{ "" };
		bool canLoadAsync{ false };
		//importer
		//dependencies
	};
}
