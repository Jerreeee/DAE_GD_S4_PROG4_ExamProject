#pragma once
#include <string>
#include <memory>
#include <map>
#include <typeindex>
#include <vector>
#include "JREngine/Scene/GameObject.h"
#include "JREngine/Scene/SceneManager.h"

namespace JRE
{
	class RendererComponentBase;
	class GameObject;
	class Scene final
	{
	public:
		explicit Scene(const std::string& name, uint32_t persistenceScope = PersistenceLayer::SceneLocal);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void SetActive(bool active);
		void Update();
		void FixedUpdate();
		void Cleanup();

		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();
		uint32_t GetPersistenceScope() const { return m_PersistenceScope; };

		template<DerivedFromComponentBase ComponentType>
		GameObject* GetGameObjectByComponentType()
		{
			for (auto& obj : m_Objects)
				if (obj->GetComponent<ComponentType>())
					return obj.get();
			return nullptr;
		}

		template<DerivedFromComponentBase ComponentType>
		ComponentType* GetComponent()
		{
			for (auto& obj : m_Objects)
				if (auto pCmp = obj->GetComponent<ComponentType>(); pCmp)
					return pCmp;
			return nullptr;
		}

		void RegisterRendererComponent(RendererComponentBase* pRendererComponent);
		void UnRegisterRendererComponent(RendererComponentBase* pRendererComponent);
		const std::vector<RendererComponentBase*>& GetRenderComponents() const { return m_RendererComponents; };
	private:
		friend class SceneManager;

		bool m_IsActive{};
		std::string m_Name;
		uint32_t m_PersistenceScope{};
		std::vector<std::unique_ptr<GameObject>> m_Objects;
		static unsigned int m_IdCounter; 
		std::vector<RendererComponentBase*> m_RendererComponents{};
	};
}
