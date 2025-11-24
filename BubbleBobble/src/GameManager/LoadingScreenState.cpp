#include "JREngine/Scene/SceneManager.h"
#include "JREngine/Core/Timer.h"

#include "GameManager/GameManagerComponent.h"
#include "LoadingScreenState.h"

using namespace JRE;

namespace BubbleBobble
{
	LoadingScreenState::LoadingScreenState(GameManagerComponent& gameManagerComponent)
		: IGameState(gameManagerComponent, "LoadingScreen")
		, m_pGameManagerComponent{ &gameManagerComponent }
	{
	}
	void LoadingScreenState::OnEnter()
	{
		m_Timer = m_MaxTime;

		m_pGameManagerComponent->PlayMusic(true);

		auto& sm = SceneManager::GetInstance();
		sm.SetNextScene(m_Name);
	}
	GameState LoadingScreenState::Update()
	{
		m_Timer -= Timer::GetInstance().GetDeltaTime();
		if (m_Timer <= 0.f)
			return GameState::InGame;
		return GameState::None;
	}
	void LoadingScreenState::OnExit()
	{
	}
}
