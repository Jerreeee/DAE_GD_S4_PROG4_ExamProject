#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

namespace JRE
{
	void SceneManager::Update()
	{
		for (auto& scene : m_scenes)
		{
			scene->Update();
		}
	}

	void SceneManager::Cleanup()
	{
		for (const auto& scene : m_scenes)
		{
			scene->Cleanup();
		}
	}

	SceneManager::SceneManager() : m_scenes() {}
	SceneManager::~SceneManager() {}

	Scene& JRE::SceneManager::CreateScene(const std::string& name)
	{
		auto scene = std::unique_ptr<Scene>(new Scene(name));
		auto pScene = scene.get();
		m_scenes.emplace_back(std::move(scene));
		m_CurrentSceneIdx = m_scenes.size() - 1;
		return *pScene;
	}
}
