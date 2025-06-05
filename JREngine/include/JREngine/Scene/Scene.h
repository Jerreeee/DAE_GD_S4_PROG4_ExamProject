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
	class Scene;
	class ISceneSystem
	{
	public:
		virtual ~ISceneSystem() = default;
		virtual void Update(Scene& scene) = 0;
	};
}

template<typename T>
concept DerivedFromISceneSystem = std::is_base_of_v<JRE::ISceneSystem, T>;

namespace JRE
{
	class RendererComponentBase;
	class GameObject;
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;


		void Start();
		void Update();
		void Cleanup();

		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();

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

		template<DerivedFromISceneSystem T>
		void AddSystem(std::unique_ptr<T> system)
		{
			std::type_index type = typeid(T);
			m_SystemMap[type] = std::move(system);
		}

		template<DerivedFromISceneSystem T>
		T* GetSystem()
		{
			std::type_index type = typeid(T);
			auto it = m_SystemMap.find(type);
			if (it != m_SystemMap.end())
				return static_cast<T*>(it->second.get());
			return nullptr;
		}

		void RegisterRendererComponent(RendererComponentBase* pRendererComponent);
		void UnRegisterRendererComponent(RendererComponentBase* pRendererComponent);
		const std::vector<RendererComponentBase*>& GetRenderComponents() const { return m_RendererComponents; };
	private:
		friend class SceneManager;

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects;
		static unsigned int m_IdCounter; 
		std::vector<RendererComponentBase*> m_RendererComponents{};
		std::map<std::type_index, std::unique_ptr<ISceneSystem>> m_SystemMap;
	};
}
