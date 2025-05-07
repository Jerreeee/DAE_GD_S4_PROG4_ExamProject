#pragma once
#include <map>
#include "JREngine/Resources/IAssetImporter.h"

namespace JRE
{
	/// <summary>
	///	Static registry that contains IAssetImporter's
	///	Meant usecase:
	///		Classes that inherit from Asset and load data from disk to construct itself
	///		should also define a separate Importer class that inherits from IAssetImporter.
	///		In the class .cpp file you should place the following:
	///			static bool s_Registered = []()
	///			{
	///			  AssetImporter::RegisterImporter(AssetType::Example, CreateRef&lt;ExampleImporter&gt;());
	///			  return true;
	///			}();
	///		This will cause all AssetImporters to be added to the static registry during static initialization
	/// </summary>
	class AssetImporter
	{
	public:
		static void RegisterImporter(AssetType type, Ref<IAssetImporter> importer);
		static Ref<IAssetImporter> GetImporter(AssetType type);

	private:
		static std::map<AssetType, Ref<IAssetImporter>>& GetImporterMap();
	};
}
