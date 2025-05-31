#include "JREngine/Scene/GameObject.h"
#include "JREngine/Asset/AssetImporter.h"
#include "JREngine/Asset/TextureImporter.h"
#include "JREngine/Asset/ResourceManager.h"

#include "Level/TileMap.h"
#include "Level/TileMapImporter.h"
#include "Level/TileMapComponent.h"
#include "SceneBuilder.h"

namespace BubbleBobble
{
	SceneBuilder::SceneBuilder(const std::filesystem::path& sceneDescriptionPath)
		: m_SceneDescriptionPath{ sceneDescriptionPath }
	{
	}
	void SceneBuilder::Build(JRE::Scene& scene)
	{
		m_pScene = &scene;

		AddTileMap();

	}
	void SceneBuilder::AddTileMap()
	{
		auto path = std::filesystem::path(m_SceneDescriptionPath / "TileMapData.txt");
		auto levelHandle = JRE::AssetImporter::GetInstance().ImportAsset(std::move(TileMapImporter(path)));
		auto levelRef = JRE::ResourceManager::GetAsset<TileMap>(levelHandle);
		auto pTileMap = std::make_unique<JRE::GameObject>("TileMap");
		auto* pComp = pTileMap->AddComponent<TileMapComponent>();
		pComp->SetLevel(levelRef);
		m_pScene->Add(std::move(pTileMap));
	}
}
