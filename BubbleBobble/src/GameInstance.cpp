#include "JREngine/Scene/Scene.h"
#include "JREngine/Scene/SceneManager.h"
#include "GameInstance.h"

namespace BubbleBobble
{
	void GameInstance::Init()
	{
		JRE::SceneManager::GetInstance().LoadScene("MainMenu");
	}
	void GameInstance::OnNotify(JRE::EventInfo&)
	{
	}
}
