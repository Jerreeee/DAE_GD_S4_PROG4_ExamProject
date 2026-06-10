#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

#include "JREngine/JREngine.h"
#include "JREngine/Core/ServiceLocator.h"
#include "JREngine/Asset/AssetRegistry.h"
#include "AssetDatabase.h"
#include "EditorResourceManager.h"

#include "Assets/AssetManifest.h"

using namespace JRE;

int main(int, char* [])
{
	fs::path dataPath = "./Data/";
	if (!fs::exists(dataPath))
		dataPath = "../Data/";

	JREngine engine{};

	// Editor phase: register all assets and write manifest to disk.
	// Cooking is metadata-only (no asset loads), so no asset loaders need to be registered here.
	AssetDatabase::GetInstance().Init(dataPath);
	ServiceLocator::RegisterResourceManager(std::make_unique<EditorResourceManager>());
	static_cast<EditorResourceManager*>(&ServiceLocator::GetResourceManager())->Init();

	BubbleBobble::AssetManifest::RegisterAll();

	const auto manifestPath = AssetRegistry::GetInstance().GetDatapath() / "asset_manifest.txt";
	AssetDatabase::GetInstance().SerializeManifest(manifestPath);

	std::cout << "Manifest written to: " << manifestPath << "\n";
	return 0;
}
