#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Cleanup()
{
	for (const auto& scene : m_scenes)
	{
		scene->Cleanup();
	}
}

dae::SceneManager::SceneManager() : m_scenes() {}
dae::SceneManager::~SceneManager() {}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	auto pScene = scene.get();
	m_scenes.emplace_back(std::move(scene));
	return *pScene;
}
