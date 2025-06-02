#include <cassert>
#include <stdexcept>
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace JRE
{
	void SceneManager::Start()
	{
		auto it = m_Scenes.find(m_StartSceneName);
		assert(it != m_Scenes.end() && "Invalid start scene name");
		LoadScene(it->first);
	}

	void SceneManager::Update()
	{
		m_Scenes[m_CurrentSceneName]->Update();
	}

	void SceneManager::Cleanup()
	{
		m_Scenes[m_CurrentSceneName]->Cleanup();
	}

	void SceneManager::SetStartSceneName(const std::string& name)
	{
		m_StartSceneName = name;
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

	Scene& SceneManager::GetCurrentScene() const
	{
		auto it = m_Scenes.find(m_CurrentSceneName);
		if (it == m_Scenes.end())
			throw std::runtime_error("No Current Scene");
		return *(it->second);
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
