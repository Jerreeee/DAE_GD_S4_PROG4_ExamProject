#include "JREngine/Scene/SceneManager.h"

#include "GameManagerComponent.h"
#include "LoadingScreenState.h"

using namespace JRE;

namespace BubbleBobble
{
	LoadingScreenState::LoadingScreenState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "LoadingScreen")
	{
	}
	void LoadingScreenState::OnEnter()
	{
		auto& sm = SceneManager::GetInstance();
		sm.LoadScene(m_Name);
	}
	GameState LoadingScreenState::Update()
	{
		return GameState::None;
	}
	void LoadingScreenState::OnExit()
	{
	}
}
