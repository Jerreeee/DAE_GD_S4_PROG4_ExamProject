#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "JREngine/Scene/Scene.h"
#include "JREngine/Core/Singleton.h"

namespace JRE
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Start();
		void Update();
		void Cleanup();

		void LoadScene(const std::string& name);
		Scene& GetCurrentScene() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'Scene'

		bool m_SceneLoaded{ false };
		std::string m_CurrentSceneName{};
		std::map<std::string, std::unique_ptr<Scene>> m_Scenes;
	};
}
