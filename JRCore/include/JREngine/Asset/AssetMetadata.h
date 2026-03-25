#pragma once
#include <filesystem>
#include <vector>
#include "JREngine/Asset/Asset.h"

namespace JRE
{
	struct AssetMetadata
	{
		// Returns "filepath/uniqueID" with forward slashes (portable, used as hash input)
		std::string GetVirtualPath() const
		{
			return (std::filesystem::path(filepath) / uniqueID).generic_string();
		}

		std::string assetType;
		std::filesystem::path filepath;   // RELATIVE path only (not absolute)
		std::string uniqueID;
		bool canLoadAsync{ false };
		std::vector<AssetHandle> dependencies;  // dep handles declared via DependsOn()
	};
}
