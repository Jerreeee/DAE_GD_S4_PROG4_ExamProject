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
#include "GameObject.h"
#include "Scene.h"
#include "TextRendererComponent.h"
#include "SpriteRendererComponent.h"
#include "FPSComponent.h"
#include "RotateParentComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>("Background Image");
	if (auto pComponent = go->AddComponent<dae::SpriteRendererComponent>(); pComponent)
	{
		pComponent->SetTexture("background.tga");
	}
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("DAE Logo");
	go->SetLocalPosition(216, 180);
	if (auto* pComponent = go->AddComponent<dae::SpriteRendererComponent>(); pComponent)
	{
		pComponent->SetTexture("logo.tga");
	}
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>("Prog4 Text");
	go->SetLocalPosition(80, 20);
	go->AddComponent<dae::TextRendererComponent>("Programming 4 Assignment", font);
	scene.Add(go);

	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = std::make_shared<dae::GameObject>("FPSCounter");
	go->AddComponent<dae::FPSComponent>(font);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>("RotatingObjectParent");
	go->SetLocalPosition(216, 180);
	scene.Add(go);

	auto go1 = std::make_shared<dae::GameObject>("RotatingObject1");
	go1->AddComponent<dae::SpriteRendererComponent>()->SetTexture("logo.tga");
	go1->AddComponent<dae::RotateParentComponent>(100.f, 90.f);
	go1->SetParent(go.get());
	scene.Add(go1);

	auto go2 = std::make_shared<dae::GameObject>("RotatingObject2");
	go2->AddComponent<dae::SpriteRendererComponent>()->SetTexture("logo.tga");
	go2->AddComponent<dae::RotateParentComponent>(50.f, 180.f);
	go2->SetParent(go1.get());
	scene.Add(go2);
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
