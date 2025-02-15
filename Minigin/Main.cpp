#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteRendererComponent.h"
#include "FPSComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>("Background Image");
	go->AddComponent<dae::SpriteRendererComponent>();
	if (go->HasComponent<dae::SpriteRendererComponent>())
	{
		go->GetComponent<dae::SpriteRendererComponent>().SetTexture("background.tga");
	}
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("DAE Logo");
	go->AddComponent<dae::SpriteRendererComponent>();
	if (go->HasComponent<dae::SpriteRendererComponent>())
	{
		go->GetComponent<dae::SpriteRendererComponent>().SetTexture("logo.tga");
	}
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>("Prog4 Text");
	go->AddComponent<dae::TextRendererComponent>("Programming 4 Assignment", font);
	if (go->HasComponent<dae::TextRendererComponent>())
	{
		go->GetComponent<dae::TextRendererComponent>().SetPosition(80, 20);
	}
	scene.Add(go);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = std::make_shared<dae::GameObject>("FPSCounter");
	go->AddComponent<dae::TextRendererComponent>("", font);
	go->AddComponent<dae::FPSComponent>();
	scene.Add(go);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}