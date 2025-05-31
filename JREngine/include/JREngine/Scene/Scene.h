#pragma once
#include <string>
#include <memory>
#include <vector>

namespace JRE
{
	class SceneManager;
	class GameObject;
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);

		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject* object);
		void RemoveAll();

		void Start();
		void Update();
		void Cleanup();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
	private:
		std::string m_name;
		std::vector<std::unique_ptr<GameObject>> m_objects;
		static unsigned int m_idCounter; 
	};
}
