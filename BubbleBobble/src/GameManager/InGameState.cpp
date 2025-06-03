#include "JREngine/Scene/SceneManager.h"

#include "GameManagerComponent.h"
#include "InGameState.h"

using namespace JRE;

namespace BubbleBobble
{
	InGameState::InGameState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "InGame")
	{
	}
	void InGameState::OnEnter()
	{
		SceneManager::GetInstance().LoadScene("Level_" + std::to_string(m_LevelIdx));
	}
	GameState InGameState::Update()
	{
		return GameState::None;
	}
	void InGameState::OnExit()
	{
	}
}
