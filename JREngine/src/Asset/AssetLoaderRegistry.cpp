#include <stdexcept>
#include "Asset/AssetLoaderRegistry.h"

namespace JRE
{
	void AssetLoaderRegistry::RegisterLoader(std::string_view typeName, LoadFunc fn)
	{
		m_Loaders.emplace(std::string(typeName), std::move(fn));
	}

	AssetRef<Asset> AssetLoaderRegistry::Load(AssetHandle handle, const AssetMetadata& metadata) const
	{
		auto it = m_Loaders.find(metadata.assetType);
		if (it == m_Loaders.end())
			throw std::runtime_error("AssetLoaderRegistry: no loader registered for type: "
			                         + std::string(metadata.assetType));
		return it->second(handle, metadata);
	}
}
