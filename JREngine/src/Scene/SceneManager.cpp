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
		m_Running = true;
	}

	void SceneManager::Update()
	{
		if (m_LoadNewScene)
			LoadNewScene();

		m_IsUpdating = true;
		m_Scenes[m_CurrentSceneName]->Update();
	}

	void SceneManager::FixedUpdate()
	{
		m_IsUpdating = true;
		m_Scenes[m_CurrentSceneName]->FixedUpdate();
	}

	void SceneManager::Cleanup()
	{
		m_Scenes[m_CurrentSceneName]->Cleanup();
		m_IsUpdating = false;
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

		if (!m_Running || !m_IsUpdating)
			LoadNewScene();
		else //defer scene loading to begin of next Update()
			m_LoadNewScene = true;
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
			EnsureObjectPersistence(oldScene, newScene);
			oldScene.Cleanup();
			oldScene.SetActive(false);
		}

		if (m_LoadCallback)
		{
			m_LoadCallback(newScene);
			m_LoadCallback = nullptr;
		}
		m_CurrentSceneName = m_NewSceneName;
		newScene.SetActive(true);
		m_SceneLoaded = true;
		m_LoadNewScene = false;
	}

	void SceneManager::EnsureObjectPersistence(Scene& srcScene, Scene& dstScene)
	{
		uint32_t dstPersistenceScope = dstScene.GetPersistenceScope();
		auto& objects = srcScene.m_Objects;

		std::vector<GameObject*> toTransfer;

		//Check which GameObjects need to be moved/destroyed
		for (auto& go : objects)
		{
			uint32_t sharedScope = go->m_PersistenceScope & dstPersistenceScope;
			bool moveToNewScene = sharedScope & ~PersistenceLayer::SceneLocal;

			if (moveToNewScene)
				toTransfer.emplace_back(go.get());
			else if (!(go->m_PersistenceScope & PersistenceLayer::SceneLocal))
				go->Destroy();
		}

		//Move GameObjects to new Scene
		for (auto* go : toTransfer)
		{
			auto pOwnedGo = srcScene.Remove(go);
			dstScene.Add(std::move(pOwnedGo));
		}
	}

	Scene& JRE::SceneManager::CreateScene(const std::string& name, uint32_t persistenceScope)
	{
		auto scene = std::unique_ptr<Scene>(new Scene(name, persistenceScope));
		auto pScene = scene.get();
		m_Scenes.emplace(name, std::move(scene));
		return *pScene;
	}
}
