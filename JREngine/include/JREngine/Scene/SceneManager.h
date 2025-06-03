#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
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

		void SetStartSceneName(const std::string& name);
		void LoadScene(const std::string& name);
		Scene& GetCurrentScene() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'Scene'

		void TransferPersistantObjects(Scene& srcScene, Scene& dstScene);

		bool m_SceneLoaded{ false };
		std::string m_CurrentSceneName{};
		std::map<std::string, std::unique_ptr<Scene>> m_Scenes;
		std::string m_StartSceneName{};
	};
}
