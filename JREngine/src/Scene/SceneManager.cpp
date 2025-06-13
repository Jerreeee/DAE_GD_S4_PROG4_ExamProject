#include <cassert>
#include <stdexcept>
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace JRE
{
	void SceneManager::Start()
	{
		assert(m_SceneLoaded && "SceneManager::SetNextScene() must be called before first Update() to load the first Scene");
	}

	void SceneManager::Update()
	{
		if (m_LoadNewScene)
			LoadNewScene();

		m_IsUpdating = true;
		m_Scenes[m_CurrentSceneName]->Update();
		m_IsUpdating = false;
	}

	void SceneManager::FixedUpdate()
	{
		m_Scenes[m_CurrentSceneName]->FixedUpdate();
	}

	void SceneManager::Cleanup()
	{
		m_Scenes[m_CurrentSceneName]->Cleanup();
	}

	void SceneManager::SetNextScene(const std::string& name, bool force)
	{
		if (name == m_CurrentSceneName) return;

		auto it = m_Scenes.find(name);
		assert(it != m_Scenes.end() && "Invalid scene name");
		m_NewSceneName = name;

		if (!force && m_IsUpdating) //defer scene loading to begin of next Update()
			m_LoadNewScene = true;
		else //immediatly load new scene
			LoadNewScene();
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

	void SceneManager::LoadNewScene()
	{
		auto it = m_Scenes.find(m_NewSceneName);
		auto& newScene = *it->second;

		if (m_SceneLoaded)
		{
			auto& oldScene = *m_Scenes[m_CurrentSceneName];
			TransferPersistantObjects(oldScene, newScene);
			oldScene.Cleanup();
		}

		m_CurrentSceneName = m_NewSceneName;
		newScene.Start();
		m_SceneLoaded = true;
		m_LoadNewScene = false;
	}

	void SceneManager::TransferPersistantObjects(Scene& srcScene, Scene& dstScene)
	{
		auto& objects = srcScene.m_Objects;
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
