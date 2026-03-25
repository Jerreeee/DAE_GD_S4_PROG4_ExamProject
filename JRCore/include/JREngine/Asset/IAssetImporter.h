#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	class IAssetImporter
	{
	public:
		virtual ~IAssetImporter() = default;
		virtual AssetMetadata GetMetadata() const = 0;

		IAssetImporter& DependsOn(AssetHandle dep)
		{
			m_Dependencies.push_back(dep);
			return *this;
		}

	protected:
		std::vector<AssetHandle> GetDeclaredDependencies() const { return m_Dependencies; }

	private:
		std::vector<AssetHandle> m_Dependencies;
	};
}
