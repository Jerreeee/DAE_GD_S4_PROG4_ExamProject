#include <algorithm>
#include "Rendering/RendererComponentBase.h"
#include "Scene/GameObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

namespace JRE
{
	unsigned int m_idCounter = 0;

	Scene::Scene(const std::string& name, uint32_t persistenceScope)
		: m_Name(name)
		, m_Objects()
		, m_PersistenceScope{ persistenceScope }
	{
	}

	Scene::~Scene() = default;

	void Scene::Add(std::unique_ptr<GameObject> object)
	{
		RegisterAllRendererComponents(object);
		m_Objects.emplace_back(std::move(object));
	}

	std::unique_ptr<GameObject> Scene::Remove(GameObject* object)
	{
		auto it = std::find_if(m_Objects.begin(), m_Objects.end(), [&](const auto& pGameObject) {
				return pGameObject.get() == object;
			});

		if (it != m_Objects.end())
		{
			(*it)->SetActive(false);
			UnegisterAllRendererComponents(*it);
			std::unique_ptr<GameObject> result = std::move(*it);
			m_Objects.erase(it);
			return result; //Return ownership
		}

		return nullptr;
	}

	void Scene::RemoveAll()
	{
		m_Objects.clear();
	}

	void Scene::SetActive(bool active)
	{
		if (m_IsActive == active) return;

		m_IsActive = active;
		for (auto& object : m_Objects)
		{
			//Only call on root objects, will trickle down from there
			if (!object->GetParent())
				object->SetActive(active);
		}
	}

	void Scene::Update()
	{
		for (auto& object : m_Objects)
		{
			if (object->IsActiveInHierarchy())
				object->Update();
		}
	}

	void Scene::FixedUpdate()
	{
		for (auto& object : m_Objects)
		{
			if (object->IsActiveInHierarchy())
				object->FixedUpdate();
		}
	}

	void JRE::Scene::Cleanup()
	{
		//Cleanup gameObjects that need to be removed
		for (const auto& object : m_Objects)
		{
			if (object->IsDestroyed())
			{
				object->SetActive(false);
				UnegisterAllRendererComponents(object);
			}
		}

		//Remove destroyed GameObjects
		std::erase_if(m_Objects, [](const std::unique_ptr<GameObject>& object) {
			return object->IsDestroyed();
			});

		//Clenaup all leftover gameObjects as they might still need to remove destroyed components
		for (const auto& object : m_Objects)
			object->Cleanup();
	}

	void Scene::RegisterAllRendererComponents(const std::unique_ptr<GameObject>& gameObject)
	{
		for (int i = 0; i < gameObject->GetComponentCount(); ++i)
		{
			auto pCmp = gameObject->GetComponentAtIndex(i);
			RendererComponentBase* pRendererCmp = dynamic_cast<RendererComponentBase*>(pCmp);
			if (pRendererCmp)
				m_RendererComponents.emplace_back(pRendererCmp);
		}
	}

	void Scene::UnegisterAllRendererComponents(const std::unique_ptr<GameObject>& gameObject)
	{
		for (int i = 0; i < gameObject->GetComponentCount(); ++i)
		{
			auto pCmp = gameObject->GetComponentAtIndex(i);
			RendererComponentBase* pRendererCmp = dynamic_cast<RendererComponentBase*>(pCmp);
			if (pRendererCmp)
			{
				auto it = std::find(m_RendererComponents.begin(), m_RendererComponents.end(), pRendererCmp);
				if (it != m_RendererComponents.end())
					m_RendererComponents.erase(it);
			}
		}
	}
}
