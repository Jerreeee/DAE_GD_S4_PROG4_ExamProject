#include <cassert>
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

namespace JRE
{
	void SceneManager::Start()
	{
		if (!m_SceneLoaded)
			LoadScene(0);
	}

	void SceneManager::Update()
	{
		m_Scenes[m_CurrentSceneIdx]->Update();
	}

	void SceneManager::Cleanup()
	{
		m_Scenes[m_CurrentSceneIdx]->Cleanup();
	}

	void SceneManager::LoadScene(size_t sceneIdx)
	{
		if (m_SceneLoaded && sceneIdx == m_CurrentSceneIdx) return;

		m_Scenes[m_CurrentSceneIdx]->Cleanup();
		assert(sceneIdx >= 0 && sceneIdx < m_Scenes.size() && "Invalid scene index");
		m_CurrentSceneIdx = sceneIdx;
		m_Scenes[m_CurrentSceneIdx]->Start();
		m_SceneLoaded = true;
	}

	SceneManager::SceneManager() : m_Scenes() {}
	SceneManager::~SceneManager() = default;

	Scene& JRE::SceneManager::CreateScene(const std::string& name)
	{
		auto scene = std::unique_ptr<Scene>(new Scene(name));
		auto pScene = scene.get();
		m_Scenes.emplace_back(std::move(scene));
		m_CurrentSceneIdx = m_Scenes.size() - 1;
		return *pScene;
	}
}
