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

	bool SceneManager::HasScene(const std::string& name)
	{
		auto it = m_Scenes.find(name);
		return it != m_Scenes.end();
	}

	void SceneManager::SetNextScene(const std::string& name, OnSceneLoadCallBack loadCallback)
	{
		if (name == m_CurrentSceneName) return;

		auto it = m_Scenes.find(name);
		assert(it != m_Scenes.end() && "Invalid scene name");
		m_NewSceneName = name;
		m_LoadCallback = loadCallback;

		if (m_IsUpdating) //defer scene loading to begin of next Update()
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
		if (m_IsUpdating) return;

		auto it = m_Scenes.find(m_NewSceneName);
		if (it == m_Scenes.end())
			throw std::runtime_error("Scene not found: " + m_NewSceneName);
		auto& newScene = *it->second;

		if (m_SceneLoaded)
		{
			auto& oldScene = *m_Scenes[m_CurrentSceneName];
			TransferPersistantObjects(oldScene, newScene);
			oldScene.Cleanup();
			oldScene.SetActive(false);
		}

		if (m_LoadCallback)
		{
			m_LoadCallback(newScene);
			m_LoadCallback = nullptr;
		}
		newScene.SetActive(true);
		m_CurrentSceneName = m_NewSceneName;
		m_SceneLoaded = true;
		m_LoadNewScene = false;
	}

	void SceneManager::TransferPersistantObjects(Scene& srcScene, Scene& dstScene)
	{
		uint32_t dstPersistenceScope = dstScene.GetPersistenceScope();
		if (!dstPersistenceScope) return;

		auto& objects = srcScene.m_Objects;
		auto it = std::remove_if(objects.begin(), objects.end(),
			[&](std::unique_ptr<GameObject>& go) {
				bool remove = go->m_PersistenceScope & dstPersistenceScope;
				if (remove)
					dstScene.Add(std::move(go));
				return remove;
			});
		objects.erase(it, objects.end());
	}

	Scene& JRE::SceneManager::CreateScene(const std::string& name, uint32_t persistenceScope)
	{
		auto scene = std::unique_ptr<Scene>(new Scene(name, persistenceScope));
		auto pScene = scene.get();
		m_Scenes.emplace(name, std::move(scene));
		return *pScene;
	}
}
