#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace Engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void Cleanup();
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'dae::Scene'

		std::vector<std::unique_ptr<Scene>> m_scenes;
	};
}
