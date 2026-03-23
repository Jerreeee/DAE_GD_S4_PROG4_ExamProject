#pragma once
#include <functional>
#include <map>
#include <string>

#include "JREngine/Core/Singleton.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	class AssetLoaderRegistry : public Singleton<AssetLoaderRegistry>
	{
	public:
		using LoadFunc = std::function<AssetRef<Asset>(AssetHandle, const AssetMetadata&)>;

		void RegisterLoader(std::string_view typeName, LoadFunc fn);
		AssetRef<Asset> Load(AssetHandle handle, const AssetMetadata& metadata) const;

	private:
		AssetLoaderRegistry() = default;
		friend class Singleton<AssetLoaderRegistry>;

		std::map<std::string, LoadFunc> m_Loaders{};
	};
}
