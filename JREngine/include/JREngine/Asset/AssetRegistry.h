#pragma once
#include <map>
#include <mutex>
#include <iosfwd>
#include "JREngine/Core/Singleton.h"
#include "JREngine/Asset/Asset.h"
#include "JREngine/Asset/IAssetImporter.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	class AssetRegistry final : public Singleton<AssetRegistry>
	{
	public:
		AssetHandle RegisterAsset(IAssetImporter&& importer);
		bool IsValidAssetHandle(AssetHandle handle) const;
		bool HasAssetAtPath(const std::string& virtualPath) const;
		AssetHandle GetHandleAtPath(const std::string& virtualPath) const;
		const AssetMetadata& GetMetadata(AssetHandle handle) const;

		void Serialize(std::ostream& out) const;
		bool Deserialize(std::istream& in);

		const std::map<AssetHandle, AssetMetadata>& GetAll() const { return m_AssetHandleToMetadata; }

	private:
		AssetRegistry() = default;
		friend class Singleton<AssetRegistry>;

		mutable std::mutex m_Mutex{};
		std::map<AssetHandle, AssetMetadata> m_AssetHandleToMetadata{};
		std::map<std::string, AssetHandle> m_PathToAssetHandle{};  // key is std::string (generic_string)
	};
}
