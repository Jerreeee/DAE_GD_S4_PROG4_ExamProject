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

		void Update();
		void Cleanup();

		Scene& GetCurrentScene() { return *(m_scenes[m_CurrentSceneIdx].get()); };
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'dae::Scene'

		size_t m_CurrentSceneIdx{};
		std::vector<std::unique_ptr<Scene>> m_scenes;
	};
}
