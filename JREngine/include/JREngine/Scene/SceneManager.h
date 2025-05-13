#pragma once
#include <vector>
#include <string>
#include <memory>
#include "JREngine/Core/Singleton.h"

namespace JRE
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Start();
		void Update();
		void Cleanup();

		void LoadScene(size_t sceneIdx);
		Scene& GetCurrentScene() { return *(m_Scenes[m_CurrentSceneIdx].get()); };
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'dae::Scene'

		bool m_SceneLoaded{ false };
		size_t m_CurrentSceneIdx{ 0 };
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
