#include "JREngine/Scene/SceneManager.h"

#include "GameManager/GameOverScreenState.h"

using namespace JRE;

namespace BubbleBobble
{
	GameOverScreenState::GameOverScreenState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "GameOverScreen")
	{
	}
	void GameOverScreenState::OnEnter()
	{
		auto& sm = SceneManager::GetInstance();
		sm.SetNextScene(m_Name);
	}
	GameState GameOverScreenState::Update()
	{
		if (m_ReturnToMainMenu)
			return GameState::MainMenu;
		return GameState::None;
	}
}
