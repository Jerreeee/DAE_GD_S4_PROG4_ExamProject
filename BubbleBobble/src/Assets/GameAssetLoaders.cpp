#include "Assets/GameAssetLoaders.h"

#include "JREngine/Asset/AssetLoaderRegistry.h"
#include "Assets/AnimsData.h"
#include "Assets/AnimsDataImporter.h"
#include "Assets/LevelData.h"
#include "Assets/LevelDataImporter.h"
#include "Assets/SceneDescriptor.h"
#include "Assets/SceneDescriptorImporter.h"
#include "TileMap/TileMap.h"
#include "TileMap/TileMapImporter.h"

using namespace JRE;

namespace BubbleBobble
{
	void RegisterGameAssetLoaders()
	{
		auto& loaders = AssetLoaderRegistry::GetInstance();
		loaders.RegisterLoader(AnimsData::GetStaticType(),       AnimDataImporter::Load);
		loaders.RegisterLoader(LevelData::GetStaticType(),       LevelDataImporter::Load);
		loaders.RegisterLoader(TileMap::GetStaticType(),         TileMapImporter::Load);
		loaders.RegisterLoader(SceneDescriptor::GetStaticType(), SceneDescriptorImporter::Load);
	}
}
