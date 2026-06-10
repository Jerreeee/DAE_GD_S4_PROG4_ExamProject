#pragma once
#include <filesystem>
#include <vector>
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	struct AssetMetadata
	{
		std::string assetType;
		std::filesystem::path filepath;   // RELATIVE path only (not absolute)
		std::string uniqueID;
		bool canLoadAsync{ false };
		std::vector<AssetHandle> dependencies;  // dep handles declared via DependsOn()
	};
}
