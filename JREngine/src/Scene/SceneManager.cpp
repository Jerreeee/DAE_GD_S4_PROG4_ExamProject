#include <cassert>
#include <stdexcept>
#include "Scene/GameObject.h"
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
		if (name == m_CurrentSceneName) return;

		auto it = m_Scenes.find(name);
		assert(it != m_Scenes.end() && "Invalid scene name");
		auto& oldScene = *m_Scenes[m_CurrentSceneName];
		auto& newScene = *it->second;

		if (m_SceneLoaded)
		{
			TransferPersistantObjects(oldScene, newScene);
			oldScene.Cleanup();
		}
		m_CurrentSceneName = name;
		newScene.Start();
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

	void SceneManager::TransferPersistantObjects(Scene& srcScene, Scene& dstScene)
	{
		auto& objects = srcScene.m_objects;
		auto it = std::remove_if(objects.begin(), objects.end(),
			[&](std::unique_ptr<GameObject>& go) {
				bool remove = go->m_Persistant;
				if (remove)
					dstScene.Add(std::move(go));
				return remove;
			});
		objects.erase(it, objects.end());
	}

	Scene& JRE::SceneManager::CreateScene(const std::string& name)
	{
		auto scene = std::unique_ptr<Scene>(new Scene(name));
		auto pScene = scene.get();
		m_Scenes.emplace(name, std::move(scene));
		return *pScene;
	}
}
