#pragma once
#include <filesystem>
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	struct AssetMetadata
	{
		inline std::filesystem::path GetVirtualPath()
		{
			return std::filesystem::path(filepath / uniqueID);
		}

		std::string assetType;
		std::filesystem::path filepath;
		std::string uniqueID;
		bool canLoadAsync{ false };
		//importer
		//dependencies
	};
}
