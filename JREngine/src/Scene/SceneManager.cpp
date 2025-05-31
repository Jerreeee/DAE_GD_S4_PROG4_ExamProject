#include <cassert>
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace JRE
{
	void SceneManager::Start()
	{
		if (!m_SceneLoaded)
		{
			auto it = m_Scenes.begin();
			LoadScene(it->first);
		}
	}

	void SceneManager::Update()
	{
		m_Scenes[m_CurrentSceneName]->Update();
	}

	void SceneManager::Cleanup()
	{
		m_Scenes[m_CurrentSceneName]->Cleanup();
	}

	void SceneManager::LoadScene(const std::string& name)
	{
		if (m_SceneLoaded && name == m_CurrentSceneName)
			return;

		m_CurrentSceneName = name;
		auto it = m_Scenes.find(name);
		if (it == m_Scenes.end())
			return;
		m_Scenes[m_CurrentSceneName]->Cleanup();
		m_CurrentSceneName = name;
		m_Scenes[m_CurrentSceneName]->Start();
		m_SceneLoaded = true;
	}

	SceneManager::SceneManager() : m_Scenes() {}
	SceneManager::~SceneManager() = default;

	Scene& JRE::SceneManager::CreateScene(const std::string& name)
	{
		auto scene = std::unique_ptr<Scene>(new Scene(name));
		auto pScene = scene.get();
		m_Scenes.emplace(name, std::move(scene));
		return *pScene;
	}
}
