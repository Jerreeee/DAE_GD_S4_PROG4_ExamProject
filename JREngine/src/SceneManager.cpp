#include "SceneManager.h"
#include "Scene.h"

void JREngine::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void JREngine::SceneManager::Cleanup()
{
	for (const auto& scene : m_scenes)
	{
		scene->Cleanup();
	}
}

JREngine::SceneManager::SceneManager() : m_scenes() {}
JREngine::SceneManager::~SceneManager() {}

JREngine::Scene& JREngine::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	auto pScene = scene.get();
	m_scenes.emplace_back(std::move(scene));
	m_CurrentSceneIdx = m_scenes.size() - 1;
	return *pScene;
}
