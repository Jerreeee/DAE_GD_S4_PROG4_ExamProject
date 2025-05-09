#pragma once
#include <functional>
#include <map>

#include <JREngine/Core/Singleton.h>
#include "JREngine/Asset/IAssetImporter.h"
#include "JREngine/Asset/AssetMetadata.h"

namespace JRE
{
	/// <summary>
	///	Registry that contains IAssetImporter's
	///	Meant usecase:
	///		Classes that inherit from Asset and load data from disk to construct itself
	///		should also define a separate Importer class that inherits from IAssetImporter.
	///		In the class .cpp file you should place the following (replace types):
	///			static bool s_Registered = []()
	///				{
	///					AssetImporter::GetInstance().RegisterImporter(AssetType::Font, FontImporter::RegisterAsset);
	///					return true;
	///				}();
	///		FontImporter::RegisterAsset must be a static method.
	///		This will cause all AssetImporters to be automatically added to the registry during static initialization.
	///		Each class in its constructor should also call AssetImporter::GetFullDatapath(filepath) and set
	///		metadata.filepath to the result.
	/// </summary>
	class AssetImporter : public Singleton<AssetImporter>
	{
	public:
		void Init(const std::filesystem::path& dataPath);

		using ImportFunc = std::function<Ref<Asset>(AssetHandle handle, const AssetMetadata& metadata)>;
		void RegisterImporter(AssetType type, ImportFunc importFunc);

		//Registers the asset in the AssetRegistry
		AssetHandle ImportAsset(IAssetImporter&& importer);
		//Imports the asset from disk
		Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);

		const std::filesystem::path& GetDatapath() const;
		std::filesystem::path GetFullDatapath(const std::filesystem::path& filepath) const;
	private:
		AssetImporter() = default;
		friend class Singleton<AssetImporter>;

		std::filesystem::path m_Datapath{};
		std::map<AssetType, ImportFunc> s_Importers{};
	};
}
