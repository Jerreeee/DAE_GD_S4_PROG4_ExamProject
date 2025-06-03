#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/ResourceManager.h"

#include "Level/TileMap.h"
#include "Level/TileMapImporter.h"
#include "Level/TileMapComponent.h"
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
	}
	void LevelBuilder::AddTileMap()
	{
		//
		std::filesystem::path relPath = std::filesystem::relative(m_Path, JRE::AssetImporter::GetInstance().GetDatapath());
		auto path = std::filesystem::path(relPath / "TileMapData.txt");
		auto tileMapHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(TileMapImporter(path)));
		auto tileMapRef = JRE::ResourceManager::GetAsset<TileMap>(tileMapHandle);
		auto pTileMap = std::make_unique<JRE::GameObject>("TileMap");
		auto* pComp = pTileMap->AddComponent<TileMapComponent>();
		pComp->SetTileMap(tileMapRef);
		m_Scene.Add(std::move(pTileMap));
	}
}
