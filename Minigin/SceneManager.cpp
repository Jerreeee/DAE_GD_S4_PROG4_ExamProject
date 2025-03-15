#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void Engine::SceneManager::Cleanup()
{
	for (const auto& scene : m_scenes)
	{
		scene->Cleanup();
	}
}

Engine::SceneManager::SceneManager() : m_scenes() {}
Engine::SceneManager::~SceneManager() {}

Engine::Scene& Engine::SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	auto pScene = scene.get();
	m_scenes.emplace_back(std::move(scene));
	return *pScene;
}
