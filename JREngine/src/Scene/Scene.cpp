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
		//Check if the gameObject has a RenderComponent
		if (auto pComponent = object->GetComponent<RendererComponentBase>(); pComponent)
			m_RendererComponents.emplace_back(pComponent);

		m_Objects.emplace_back(std::move(object));
	}

	void Scene::Remove(GameObject* object)
	{
		m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
			[&](const auto& pGameObject)
			{
				return pGameObject.get() == object;
			}
		), m_Objects.end());
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
		//Scene is responsible for removing ALL gameobjects, also child gameobjects
		m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(),
			[](const auto& object)
			{
				return object->IsDestroyed();
			}
		), m_Objects.end());

		//Extra Cleanup loop over the leftover gameobjects as they might need to remove components marked for destroying
		for (const auto& object : m_Objects)
		{
			object->Cleanup();
		}
	}

	void JRE::Scene::RegisterRendererComponent(RendererComponentBase* pRendererComponent)
	{
		m_RendererComponents.emplace_back(pRendererComponent);
	}

	void JRE::Scene::UnRegisterRendererComponent(RendererComponentBase* pRendererComponent)
	{
		m_RendererComponents.erase(std::find(m_RendererComponents.begin(), m_RendererComponents.end(), pRendererComponent));
	}
}
