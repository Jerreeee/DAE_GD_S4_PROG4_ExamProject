#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include "JREngine/Scene/PersistenceLayer.h"
#include "JREngine/Core/Singleton.h"

namespace JRE
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, uint32_t persistenceScope = PersistenceLayer::SceneLocal);

		void Start();
		void Update();
		void FixedUpdate();
		void Cleanup();

		bool HasScene(const std::string& name);
		using OnSceneLoadCallBack = std::function<void(Scene& scene)>;
		void SetNextScene(const std::string& name, OnSceneLoadCallBack loadCallback = {});
		Scene& GetCurrentScene() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		~SceneManager(); //suppress error: use of undefined type 'Scene'

		void LoadNewScene();
		void EnsureObjectPersistence(Scene& srcScene, Scene& dstScene);

		bool m_IsUpdating{};
		bool m_SceneLoaded{ false };
		bool m_LoadNewScene{};
		std::string m_CurrentSceneName{};
		std::string m_NewSceneName{};
		OnSceneLoadCallBack m_LoadCallback{};
		std::map<std::string, std::unique_ptr<Scene>> m_Scenes;
	};
}
