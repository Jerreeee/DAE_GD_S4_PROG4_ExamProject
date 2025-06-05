#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/ResourceManager.h"

#include "TileMap/TileMap.h"
#include "TileMap/TileMapImporter.h"
#include "TileMap/TileMapComponent.h"
#include "Assets/LevelDataImporter.h"
#include "Assets/LevelDataComponent.h"
#include "LevelBuilder.h"

using namespace JRE;

namespace BubbleBobble
{
	LevelBuilder::LevelBuilder(JRE::Scene& scene, const std::filesystem::path& path)
		: m_Scene{ scene }
		, m_Path{ path }
	{
	}
	void LevelBuilder::Build()
	{
		AddTileMap();
		AddLevelData();
	}
	void LevelBuilder::AddTileMap()
	{
		std::filesystem::path relPath = std::filesystem::relative(m_Path, JRE::AssetImporter::GetInstance().GetDatapath());
		auto path = std::filesystem::path(relPath / "TileMapData.txt");
		auto tileMapHandle = AssetImporter::GetInstance().ImportAsset(std::move(TileMapImporter(path)));
		auto tileMapRef = ResourceManager::GetAsset<TileMap>(tileMapHandle);
		auto pTileMap = std::make_unique<GameObject>("TileMap");
		auto* pComp = pTileMap->AddComponent<TileMapComponent>();
		pComp->SetTileMap(tileMapRef);
		m_Scene.Add(std::move(pTileMap));
	}
	void LevelBuilder::AddLevelData()
	{
		std::filesystem::path relPath = std::filesystem::relative(m_Path, JRE::AssetImporter::GetInstance().GetDatapath());
		auto path = std::filesystem::path(relPath / "LevelData.txt");
		auto levelDataHandle = AssetImporter::GetInstance().ImportAsset(std::move(LevelDataImporter(path)));
		auto levelDataRef = ResourceManager::GetAsset<LevelData>(levelDataHandle);
		auto pLevelData = std::make_unique<GameObject>("LevelData");
		auto* pComp = pLevelData->AddComponent<LevelDataComponent>();
		pComp->m_LevelData = levelDataRef;
		m_Scene.Add(std::move(pLevelData));
	}
}
