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
		void FixedUpdate();
		void Cleanup();

		void SetNextScene(const std::string& name, bool force = false);
		Scene& GetCurrentScene() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'Scene'

		void LoadNewScene();
		void TransferPersistantObjects(Scene& srcScene, Scene& dstScene);

		bool m_IsUpdating{};
		bool m_SceneLoaded{ false };
		bool m_LoadNewScene{};
		std::string m_CurrentSceneName{};
		std::string m_NewSceneName{};
		std::map<std::string, std::unique_ptr<Scene>> m_Scenes;
	};
}
