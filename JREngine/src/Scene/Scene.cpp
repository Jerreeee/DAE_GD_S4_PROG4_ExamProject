#include <algorithm>
#include "Scene/GameObject.h"
#include "JREngine/Scene/SceneManager.h"
#include "Scene/Scene.h"

namespace JRE
{
	unsigned int m_idCounter = 0;

	Scene::Scene(const std::string& name) :
		m_name(name),
		m_objects()
	{
	}

	Scene::~Scene() = default;

	void Scene::Add(std::unique_ptr<GameObject> object)
	{
		m_objects.emplace_back(std::move(object));
	}

	void Scene::Remove(GameObject* object)
	{
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
			[&](const auto& pGameObject)
			{
				return pGameObject.get() == object;
			}
		), m_objects.end());
	}

	void Scene::RemoveAll()
	{
		m_objects.clear();
	}

	void Scene::Start()
	{
		for (auto& object : m_objects)
		{
			object->Start();
		}
	}

	void Scene::Update()
	{
		for (auto& object : m_objects)
		{
			object->Update();
		}
	}

	void JRE::Scene::Cleanup()
	{
		//Scene is responsible for removing ALL gameobjects, also child gameobjects
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
			[](const auto& object)
			{
				return object->IsDestroyed();
			}
		), m_objects.end());

		//Extra Cleanup loop over the leftover gameobjects as they might need to remove components marked for destroying
		for (const auto& object : m_objects)
		{
			object->Cleanup();
		}
	}
}
